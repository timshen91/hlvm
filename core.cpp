// ---------- file scope ----------
class FileNode : public ASTNode {
 public:
  void codegen() const;

  vector<ASTNodePtr> children;
};

void FileNode::codegen() const {
  for (auto &iter : children) {
    if (iter != nullptr) iter->codegen();
  }
}

ASTNodePtr handle_file(const List &list) {
  auto &children = list.get_children();
  auto file = make_unique<FileNode>();
  env_stack.push_back("file");
  {
    for (long i = 1; i < static_cast<long>(children.size()); i++) {
      auto &ch = children[i]->get_children();
      ensure(ch.size() > 0, "");
      auto &kwd = ch[0]->get_string();
      file->children.push_back(parse(*children[i]));
    }
  }
  env_stack.pop_back();
  return unique_ptr<ASTNode>(file.release());
}

// ---------- comment ----------
ASTNodePtr handle_comment(const List &list) { return nullptr; }

// ---------- function ----------

typedef String Type;

struct FunctionNode : public ASTNode {
  String name;
  map<String, Type> arg_list;
  Type ret_type;
  vector<ASTNodePtr> body;

  void codegen() const;
};

void FunctionNode::codegen() const {}

ASTNodePtr handle_function(const List &list) {
  ensure(env_table[env_stack.back()].count("function"),
         "Invalid keyword under environment " + env_stack.back());
  cerr << "handle_function\n";
  auto func = make_unique<FunctionNode>();
  auto &children = list.get_children();
  ensure(children.size() >= 4, "`function` require at least 4 arguments");
  ensure(children[0]->get_string() == "function", "Keyword is not `function`");
  func->name = children[1]->get_string();
  auto &args = func->arg_list;
  for (auto &iter : children[2]->get_children()) {
    auto &arg = iter->get_children();
    ensure(arg.size() == 2, "Invalid argument list");
    Type type = arg[0]->get_string();
    auto name = arg[1]->get_string();
    ensure(args.count(name) == 0, "Duplicated argument");
    args[name] = type;
  }
  func->ret_type = children[3]->get_string();
  env_stack.push_back("function");
  {
    for (long i = 4; i < static_cast<long>(children.size()); i++) {
      func->body.push_back(parse(*children[i]));
    }
  }
  env_stack.pop_back();
  return unique_ptr<ASTNode>(func.release());
}

ASTNodePtr handle_if(const List &list) {
  ensure(env_table[env_stack.back()].count("if"),
         "Invalid keyword under environment " + env_stack.back());
  cerr << "handle_if\n";
  return nullptr;
}

ASTNodePtr handle_lt(const List &list) {
  cerr << "handle_lt\n";
  return nullptr;
}

ASTNodePtr handle_return(const List &list) {
  ensure(env_table[env_stack.back()].count("return"),
         "Invalid keyword under environment " + env_stack.back());
  cerr << "handle_return\n";
  return nullptr;
}

ASTNodePtr handle_plus(const List &list) {
  cerr << "handle_plus\n";
  return nullptr;
}

ASTNodePtr handle_call(const List &list) {
  cerr << "handle_call\n";
  return nullptr;
}

ASTNodePtr handle_minus(const List &list) {
  cerr << "handle_minus\n";
  return nullptr;
}

map<String, Handler> handler = {
  { "file", handle_file }, { "#", handle_comment },
  { "function", handle_function }, { "if", handle_if }, { "<=", handle_lt },
  { "return", handle_return }, { "+", handle_plus }, { "call", handle_call },
  { "-", handle_minus },
};
