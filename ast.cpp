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

typedef unique_ptr<ExprNode> ExprPtr;

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
  vector<ExprPtr> args;

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
  ExprPtr cond;
  NodePtr then_branch;
  NodePtr else_branch;

  void codegen() const { cerr << "if node codegen\n"; }
};

class ReturnNode : public Node {
 public:
  NodePtr ret;

  void codegen() const { cerr << "return node codegen\n"; }
};
