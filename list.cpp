List::List(NodeType type) : type(type) {}
List::List(String data) : type(NodeType::str), data(data) {}

void List::append(ListPtr &&t) { children.push_back(move(t)); }

static void print_depth(int depth) {
  while (depth--)
    cout << "  ";
}

const String &List::get_string() const {
  ensure(type == NodeType::str, "Symbol expected");
  return data;
}

const vector<ListPtr> &List::get_children() const {
  ensure(type == NodeType::list, "List expected");
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
