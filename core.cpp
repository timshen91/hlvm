class Type {
    String name;
    int size;
};

class Variable {
    String name;
    Type type;
};

class FunctionNode : public Node {
public:
    String name;
    Type return_type;
    vector<Variable> args; // arguments
    vector<NodePtr> stmts; // statements

    void codegen() const {
	cerr << "function codegen\n";
    }
};

typedef map<String, Variable> Environment;
Environment global_variable;
map<String, FunctionNode*> global_function;
vector<Environment*> env_stack;

class FileNode : public Node {
public:
    vector<NodePtr> decls; // declarations

    void codegen() const {
	cerr << "file codegen\n";
	for (const auto& it : decls) {
	    it->codegen();
	}
    }
};

// ---------- file scope ----------
NodePtr handle_file(const List& list) {
  cerr << "handle_file\n";
  const auto& vec = list.get_children();
  auto filenode = new FileNode();
  env_stack.push_back(&global_variable);
  for (long i = 1; i < (long)vec.size(); i++) {
      auto p = parse(*vec[i]);
      if (p != nullptr) {
	  filenode->decls.push_back(move(p));
      }
  }
  env_stack.pop_back();
  return unique_ptr<Node>(filenode);
}

// ---------- comment ----------
NodePtr handle_comment(const List& list) { return nullptr; }

// ---------- function ----------
NodePtr handle_function(const List& list) {
  cerr << "handle_function\n";
  auto funcnode = new FunctionNode();
  return unique_ptr<Node>(funcnode);
}

NodePtr handle_if(const List& list) {
  cerr << "handle_if\n";
  return nullptr;
}

NodePtr handle_lt(const List& list) {
  cerr << "handle_lt\n";
  return nullptr;
}

NodePtr handle_return(const List& list) {
  cerr << "handle_return\n";
  return nullptr;
}

NodePtr handle_plus(const List& list) {
  cerr << "handle_plus\n";
  return nullptr;
}

NodePtr handle_call(const List& list) {
  cerr << "handle_call\n";
  return nullptr;
}

NodePtr handle_minus(const List& list) {
  cerr << "handle_minus\n";
  return nullptr;
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
