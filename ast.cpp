class Node {
 public:
  virtual void codegen() const = 0;
  virtual ~Node() {}
};

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

// int a() {
//     {
//       int b;
//       b = 0;
//     }
// }
// A block node is a block with an independent env and a sequence of statements,
// for example, b is in a explicit block.
// A function by default contains a block for its own statemnts, local variables
// and parameters.
class BlockNode : public Node {
 public:
  Environment env;
  vector<NodePtr> stmts;  // statements

  BlockNode(Environment* parent) : env(parent) {}

  void codegen() const;
};

class FunctionNode : public Node {
 public:
  String name;
  Type* return_type;
  vector<Variable*> params;  // parameters
  BlockNode body;

  FunctionNode(String name, Environment* parent) : name(name), body(parent) {}
  
  void codegen() const;
};

// A file is nothing more than a set of definitions.
class FileNode : public Node {
 public:
  Environment env;

  FileNode(Environment* parent) : env(parent) {}

  void codegen() const;
};

// An expr is either a function call or an atom (a symbol).
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

  void codegen() const;
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

  void codegen() const;
};

class IfNode : public Node {
 public:
  ExprPtr cond;
  NodePtr then_branch;
  NodePtr else_branch;

  void codegen() const;
};

class ReturnNode : public Node {
 public:
  NodePtr ret;

  void codegen() const;
};

#include "codegen.cpp"
