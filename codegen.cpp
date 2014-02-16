#ifndef __codegen_cpp_included__
#define __codegen_cpp_included__

#include <cstdio>
#include <string>
#include <sstream>

using namespace std;

class GlobalLabeller {
private:
  int counter;

  GlobalLabeller (): counter (0) {};

public:
  static GlobalLabeller& getInstance () {
    static GlobalLabeller instance;
    return instance;
  }

  string next_label () {
    stringstream ss;
    ss << "L" << counter++;
    return ss.str();
  }
};

GlobalLabeller gl = GlobalLabeller::getInstance();

void FileNode::codegen () const {
  cerr << "file node codegen\n";
  for (auto& it : env.functions) {
    it.second->codegen();
  }
  cerr << "file node codgen complete\n";
}

void AtomNode::codegen () const {
  cerr << "atom node codegen\n";
  cerr << "atom node codegen complete\n";
}

void CallNode::codegen () const {
  cerr << "call node codegen\n";
  cerr << "call node codegen complete\n";
}

void IfNode::codegen () const {
  cerr << "if node codegen\n";
  cond->codegen();
  string tbl = gl.next_label(); // then branch label
  string ebl = gl.next_label(); // else branch label
  string fil = gl.next_label(); // end if label
  // assuming the result of cond expr is put into %rax
  cout << "\tcmpq\t$0, %rax\n"
       << "\tje\t" << ebl << '\n'
       << tbl << ":\n";
  then_branch->codegen();
  cout << "\tjmp\t" << fil << '\n'
       << ebl << ':' << endl;
  else_branch->codegen();
  cout << fil << ":\n";
  cerr << "if node codegen complete\n";
}

void ReturnNode::codegen () const {
  cerr << "return node codegen\n";
  cerr << "return node codegen complete\n";
}

void FunctionNode::codegen () const {
  cerr << "function node codegen\n";
  cerr << "function node codegen complete\n";
}

void BlockNode::codegen () const {
  cerr << "block node codegen\n";
  cerr << "block node codegen complete\n";
}

#endif
