NodePtr handle_file(const List& list) {
  cerr << "handle_file\n";
  auto env = env_stack.back();
  auto file = make_unique<FileNode>(env);
  env_stack.push_back(&file->env);
  for (size_t i = 1; i < list.size(); i++) {
    parse(list[i]);
  }
  env_stack.pop_back();
  return NodePtr(file.release());
}

NodePtr handle_comment(const List& list) { return nullptr; }

NodePtr handle_function(const List& list) {
  cerr << "handle_function\n";
  auto env = env_stack.back();
  ensure(list.size() >= 5, "Function requires 5 components");
  auto funcname = list[1].get_symbol();
  auto func = make_unique<FunctionNode>(funcname, env);
  auto funcenv = &func->body.env;
  ensure(env == funcenv->parent, "Internal");
  for (size_t i = 0; i < list[2].size(); i++) {
    ensure(list[2][i].size() == 2, "Invalid parameter declaration");
    auto type = funcenv->must_lookup_type(list[2][i][0].get_symbol());
    auto name = list[2][i][1].get_symbol();
    func->params.push_back(
        funcenv->create_var(make_unique<Variable>(name, type)));
  }
  func->return_type = funcenv->must_lookup_type(list[3].get_symbol());
  env_stack.push_back(funcenv);
  for (size_t i = 4; i < list.size(); i++) {
    func->body.stmts.push_back(parse(list[i]));
  }
  env_stack.pop_back();
  env->create_func(move(func));
  return nullptr;
}

NodePtr handle_return(const List& list) {
  cerr << "handle_return\n";
  ensure(list.size() == 2, "return statement requires 2 components");
  ensure(list[1][0].get_symbol() == "expr",
         "return statement requires an expr to return");
  auto ret = make_unique<ReturnNode>();
  ret->ret = parse(list[1]);
  return NodePtr(ret.release());
}

ExprPtr handle_expr_impl(const List& list) {
  auto env = env_stack.back();
  if (list.type == NodeType::symbol) {
    return ExprPtr(new AtomNode(env->must_lookup_var(list.get_symbol())));
  } else {
    ensure(list.size() >= 1, "Function name required");
    auto call =
        make_unique<CallNode>(env->must_lookup_func(list[0].get_symbol()));
    for (size_t i = 1; i < list.size(); i++) {
      call->args.push_back(handle_expr_impl(list[i]));
    }
    call->type_check();
    return ExprPtr(call.release());
  }
}

NodePtr handle_expr(const List& list) {
  cerr << "handle_expr\n";
  return NodePtr(handle_expr_impl(list[1]).release());
}

NodePtr handle_if(const List& list) {
  cerr << "handle_if\n";
  auto env = env_stack.back();
  ensure(list.size() == 4, "if statement requires 4 components");
  auto ifnode = make_unique<IfNode>();
  ensure(list[1][0].get_symbol() == "expr",
         "if statement requires an expr as condition");
  ifnode->cond = handle_expr_impl(list[1][1]);
  ensure(ifnode->cond->get_type() == env->must_lookup_type("int"),
         "int expected");
  ifnode->then_branch = parse(list[2]);
  ifnode->else_branch = parse(list[3]);
  return NodePtr(ifnode.release());
}

// state required ("-" is any), set state to ("-" is no changed), and the
// handler function.
typedef tuple<String, String, Handler> HandlerEntry;

map<String, HandlerEntry> handler = {
    {"file", HandlerEntry{"root_scope", "file_scope", handle_file}},
    {"#", HandlerEntry{"-", "-", handle_comment}},
    {"function", HandlerEntry{"file_scope", "block_scope", handle_function}},
    {"if", HandlerEntry{"block_scope", "-", handle_if}},
    {"return", HandlerEntry{"block_scope", "-", handle_return}},
    {"expr", HandlerEntry{"block_scope", "-", handle_expr}}, };

void init() { top_env.create_type(make_unique<Type>("int", 4)); }
