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
    int size;
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
};

class BlockNode {
public:
    Environment env;
    vector<NodePtr> stmts; // statements

    BlockNode(Environment* parent) : env(parent) {}
};

class FunctionNode {
public:
    String name;
    Type* return_type;
    vector<Variable*> args; // arguments
    BlockNode body;

    FunctionNode(Environment* parent) : body(parent) {}
};

class FileNode {
public:
    Environment env;

    FileNode() : env(nullptr) {}

    void codegen() const {
	cerr << "file codegen\n";
    }
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
  ensure(env->functions.count(funcname) == 0, "Duplicate function definition");
  FunctionNode* func = &env->functions.emplace(piecewise_construct, forward_as_tuple(funcname), forward_as_tuple(env)).first->second;
  func->name = funcname;
  env = &func->body.env;
  for (size_t i = 0; i < list[2].size(); i++) {
      auto type = list[2][i][0].get_symbol();
      auto name = list[2][i][1].get_symbol();
      cerr << type << " " << name << "\n";
      // TODO insert arguments.
  }
}

void handle_if(void* generic, const List& list) {
  cerr << "handle_if\n";
}

void handle_lt(void* generic, const List& list) {
  cerr << "handle_lt\n";
}

void handle_return(void* generic, const List& list) {
  cerr << "handle_return\n";
}

void handle_plus(void* generic, const List& list) {
  cerr << "handle_plus\n";
}

void handle_call(void* generic, const List& list) {
  cerr << "handle_call\n";
}

void handle_minus(void* generic, const List& list) {
  cerr << "handle_minus\n";
}

map<String, Handler> handler = {{"file", handle_file},
                                {"#", handle_comment},
                                {"function", handle_function},
                                {"if", handle_if},
                                {"<=", handle_lt},
                                {"return", handle_return},
                                {"+", handle_plus},
                                {"call", handle_call},
                                {"-", handle_minus}, };
