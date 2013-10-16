List::List(NodeType type) : type(type) {}
List::List(NodeType type, String data) : type(type), data(data) {}

void List::append(ListPtr &&t) { children.push_back(move(t)); }

ValuePtr List::codegen() const {
  ensure(type == NodeType::list, "List type expected");
  ensure(children.size() > 0, "");
  ensure(handler.count(children[0]->data) > 0,
         String("No such keyword: " + children[0]->data));
  return handler[children[0]->data](*this);
}

static void print_depth(int depth) {
  while (depth--) cout << "  ";
}

const String &List::get_string() const {
  ensure(type == NodeType::str, "");
  return data;
}

const vector<ListPtr> &List::get_children() const {
  ensure(type == NodeType::list, "");
  return children;
}

void List::dump() const {
  static int depth = 0;
  print_depth(depth);
  switch (type) {
    case NodeType::list:
      cout << "(\n";
      depth++;
      for (auto &it : children) {
        it->dump();
        cout << "\n";
      }
      depth--;
      print_depth(depth);
      cout << ")";
      break;
    case NodeType::str:
      cout << data;
      break;
  }
}
