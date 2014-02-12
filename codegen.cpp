void FileNode::codegen () const {
  cerr << "file node codegen\n";
  for (auto& it : env.functions) {
    it.second -> codegen();
  }
}

void AtomNode::codegen () const {
  cerr << "atom node codegen\n";
}

void CallNode::codegen () const {
  cerr << "call node codegen\n";
}

void IfNode::codegen () const {
  cerr << "if node codegen\n";
}

void ReturnNode::codegen () const {
  cerr << "return node codegen\n";
}

void FunctionNode::codegen () const {
  cerr << "function node codegen\n";
}

void BlockNode::codegen () const {
  cerr << "block node codegen\n";
}
