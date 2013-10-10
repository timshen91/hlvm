List::List(NodeType type) : type(type) {}
List::List(NodeType type, String data) : type(type), data(data) {}

void List::append(ListPtr&& t) {
  children.push_back(move(t));
}

Code List::codegen() const {
  switch (type) {
  case NodeType::list:
    assert(children.size() > 0);
    if (callback.count(children[0]->data) == 0)
      error(String("No such keyword: " + children[0]->data));
    return callback[children[0]->data](*this);
  case NodeType::str:
    return data;
  }
  error("Unreachable");
  return "";
}

static void print_depth(int depth) {
  while (depth--) cout << "  ";
}

const String& List::get_string() const {
  assert(type == NodeType::str);
  return data;
}

const vector<ListPtr>& List::get_children() const {
  assert(type == NodeType::list);
  return children;
}

void List::dump() const {
  static int depth = 0;
  print_depth(depth);
  switch (type) {
  case NodeType::list:
    cout << "(\n";
    depth++;
    for (auto& it : children) {
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
