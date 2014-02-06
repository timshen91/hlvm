class Type {
 public:
  String name;
  size_t size;

  Type(String name, size_t size) : name(name), size(size) {}
};

class Variable {
 public:
  String name;
  Type* type;

  Variable(String name, Type* type) : name(name), type(type) {}
};

class FunctionNode;
class Environment {
 public:
  map<String, unique_ptr<Variable>> vars;
  map<String, unique_ptr<Type>> types;
  map<String, unique_ptr<FunctionNode>> functions;
  Environment* parent;

  Environment(Environment* parent) : parent(parent) {}

  Variable* create_var(unique_ptr<Variable>&& ptr) {
    return create_impl(vars, move(ptr));
  }

  FunctionNode* create_func(unique_ptr<FunctionNode>&& ptr) {
    return create_impl(functions, move(ptr));
  }

  Type* create_type(unique_ptr<Type>&& ptr) {
    return create_impl(types, move(ptr));
  }

  Variable* must_lookup_var(String name) const {
    return must_lookup<Variable>(offsetof(Environment, vars), name);
  }

  FunctionNode* must_lookup_func(String name) const {
    return must_lookup<FunctionNode>(offsetof(Environment, functions), name);
  }

  Type* must_lookup_type(String name) const {
    return must_lookup<Type>(offsetof(Environment, types), name);
  }

 private:
  template <typename T>
  T* create_impl(map<String, unique_ptr<T>>& c, unique_ptr<T>&& ptr) {
    ensure(c.count(ptr->name) == 0, "Duplicated definition");
    return &*(c[ptr->name] = move(ptr));
  }

  template <typename T>
  T* must_lookup(size_t member_offset, String name) const {
    ensure(this, String("`" + name + "` not defined"));
    const auto& c = *static_cast<map<String, unique_ptr<T>>*>(
                         (void*)((char*)this + member_offset));
    if (c.count(name) == 0) {
      return parent->must_lookup<T>(member_offset, name);
    }
    return &*c.at(name);
  }
};

class BlockNode {
 public:
  Environment env;
  vector<NodePtr> stmts;  // statements

  BlockNode(Environment* parent) : env(parent) {}
};

class FunctionNode {
 public:
  String name;
  Type* return_type;
  vector<Variable*> params;  // parameters
  BlockNode body;

  FunctionNode(String name, Environment* parent) : name(name), body(parent) {}
};

class FileNode : public Node {
 public:
  Environment env;

  FileNode(Environment* parent) : env(parent) {}

  void codegen() const { cerr << "file codegen\n"; }
};

class ExprNode : public Node {
 public:
  virtual Type* get_type() const = 0;
  virtual void type_check() const = 0;
  void codegen() const = 0;
};

class AtomNode : public ExprNode {
 public:
  Variable* var;

  AtomNode(Variable* var) : var(var) {}

  Type* get_type() const { return var->type; }

  void type_check() const {}

  void codegen() const {}
};

class CallNode : public ExprNode {
 public:
  FunctionNode* func;
  vector<unique_ptr<ExprNode>> args;

  CallNode(FunctionNode* func) : func(func) {}

  Type* get_type() const { return func->return_type; }

  void type_check() const {
    const auto& params = func->params;
    ensure(params.size() == args.size(), "Arguments mismatch");
    for (size_t i = 0; i < params.size(); i++) {
      ensure(params[i]->type == args[i]->get_type(), "Type mismatch");
    }
  }

  void codegen() const {}
};

class IfNode : public Node {
 public:
  NodePtr cond;
  NodePtr then_branch;
  NodePtr else_branch;

  void codegen() const { cerr << "if node codegen\n"; }
};

class ReturnNode : public Node {
 public:
  NodePtr ret;

  void codegen() const { cerr << "return node codegen\n"; }
};

NodePtr handle_file(Environment* env, const List& list) {
  cerr << "handle_file\n";
  auto file = make_unique<FileNode>(env);
  for (size_t i = 1; i < list.size(); i++) {
    parse(&file->env, list[i]);
  }
  return unique_ptr<Node>(file.release());
}

NodePtr handle_comment(Environment* env, const List& list) { return nullptr; }

NodePtr handle_function(Environment* env, const List& list) {
  cerr << "handle_function\n";
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
  for (size_t i = 4; i < list.size(); i++) {
    func->body.stmts.push_back(parse(funcenv, list[i]));
  }
  env->create_func(move(func));
  return nullptr;
}

NodePtr handle_if(Environment* env, const List& list) {
  cerr << "handle_if\n";
  ensure(list.size() == 4, "if statement requires 4 components");
  ensure(list[1][0].get_symbol() == "expr",
         "if statement requires an expr as condition");
  auto ifnode = make_unique<IfNode>();
  ifnode->cond = parse(env, list[1]);
  ifnode->then_branch = parse(env, list[2]);
  ifnode->else_branch = parse(env, list[3]);
  return unique_ptr<Node>(ifnode.release());
}

NodePtr handle_return(Environment* env, const List& list) {
  cerr << "handle_return\n";
  ensure(list.size() == 2, "return statement requires 2 components");
  ensure(list[1][0].get_symbol() == "expr",
         "return statement requires an expr to return");
  auto ret = make_unique<ReturnNode>();
  ret->ret = parse(env, list[1]);
  return unique_ptr<Node>(ret.release());
}

unique_ptr<ExprNode> handle_expr_impl(Environment* env, const List& list) {
  if (list.type == NodeType::symbol) {
    return unique_ptr<ExprNode>(
        new AtomNode(env->must_lookup_var(list.get_symbol())));
  } else {
    ensure(list.size() >= 1, "Function name required");
    auto call =
        make_unique<CallNode>(env->must_lookup_func(list[0].get_symbol()));
    for (size_t i = 1; i < list.size(); i++) {
      call->args.push_back(handle_expr_impl(env, list[i]));
    }
    call->type_check();
    return unique_ptr<ExprNode>(call.release());
  }
}

NodePtr handle_expr(Environment* env, const List& list) {
  cerr << "handle_expr\n";
  return unique_ptr<Node>(handle_expr_impl(env, list[1]).release());
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
