// ---------- AST ----------
class Node {
 public:
  virtual void codegen() const = 0;
  virtual ~Node() {}
};

// FIXME Will be reimplemented in the future.
typedef unique_ptr<Node> NodePtr;

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
  map<String, Variable> vars;
  map<String, Type> types;
  map<String, FunctionNode> functions;
  Environment* parent;

  Environment(Environment* parent) : parent(parent) {}

  template <typename... Args>
  Variable* emplace_var(Args&&... args) {
    return emplace_impl(vars, args...);
  }

  template <typename... Args>
  FunctionNode* emplace_func(Args&&... args) {
    return emplace_impl(functions, args...);
  }

  template <typename... Args>
  Type* emplace_type(Args&&... args) {
    return emplace_impl(types, args...);
  }

  Type* must_lookup_type(String name) {
    ensure(this, "No such type");
    if (types.count(name) == 0) {
      return parent->must_lookup_type(name);
    }
    return &types.find(name)->second;
  }

 private:
  template <typename T, typename... Args>
  T* emplace_impl(map<String, T>& c, String name, Args&&... args) {
    auto res = c.emplace(piecewise_construct, forward_as_tuple(name),
                         forward_as_tuple(name, args...));
    ensure(res.second, "Duplicated definition");
    return &res.first->second;
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
  vector<Variable*> args;  // arguments
  BlockNode body;

  FunctionNode(String name, Environment* parent) : name(name), body(parent) {}
};

class FileNode {
 public:
  Environment env;

  FileNode() : env(nullptr) { env.emplace_type("int", 4); }

  void codegen() const { cerr << "file codegen\n"; }
};

void handle_file(void* generic, const List& list) {
  cerr << "handle_file\n";
  for (size_t i = 1; i < list.size(); i++) {
    parse(&static_cast<FileNode*>(generic)->env, list[i]);
  }
}

void handle_comment(void* generic, const List& list) {}

void handle_function(void* generic, const List& list) {
  cerr << "handle_function\n";
  auto env = static_cast<Environment*>(generic);
  auto funcname = list[1].get_symbol();
  FunctionNode* func = env->emplace_func(funcname, env);
  env = &func->body.env;
  for (size_t i = 0; i < list[2].size(); i++) {
    ensure(list[2][i].size() == 2, "Invalid argument declaration");
    auto type = env->must_lookup_type(list[2][i][0].get_symbol());
    auto name = list[2][i][1].get_symbol();
    func->args.push_back(env->emplace_var(name, type));
  }
  auto type = env->must_lookup_type(list[3].get_symbol());
  func->return_type = type;
  for (size_t i = 4; i < list.size(); i++) {
    parse(&func->body, list[i]);
  }
}

void handle_if(void* generic, const List& list) { cerr << "handle_if\n"; }

void handle_lt(void* generic, const List& list) { cerr << "handle_lt\n"; }

void handle_return(void* generic, const List& list) {
  cerr << "handle_return\n";
}

void handle_plus(void* generic, const List& list) { cerr << "handle_plus\n"; }

void handle_call(void* generic, const List& list) { cerr << "handle_call\n"; }

void handle_minus(void* generic, const List& list) { cerr << "handle_minus\n"; }

map<String, Handler> handler = {{"file", handle_file},
                                {"#", handle_comment},
                                {"function", handle_function},
                                {"if", handle_if},
                                {"<=", handle_lt},
                                {"return", handle_return},
                                {"+", handle_plus},
                                {"call", handle_call},
                                {"-", handle_minus}, };
