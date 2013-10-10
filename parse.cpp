ListPtr parse() {
  static char ch = '\n';
  while (isspace(ch)) ch = cin.get();
  if (cin.eof()) return nullptr;
  ListPtr list;
  if (ch == '(') {
    ch = cin.get();
    list = make_unique<List>(NodeType::list);
    while (ch != ')') {
      ensure(!cin.eof(), "Unexpected EOF");
      list->append(parse());
    }
    ch = cin.get();
  } else if (ch == ')') {
    ensure(false, "Unmatched ')'");
  } else {
    String data;
    while (!cin.eof() && !isspace(ch) && ch != ')') {
      data += ch;
      ch = cin.get();
    }
    list = make_unique<List>(NodeType::str, data);
  }
  while (isspace(ch)) ch = cin.get();
  return list;
}

int main(int argc, char* args[]) {
  init_core();
  if (argc >= 2) {
    freopen(args[1], "r", stdin);
  }
  ListPtr list;
  while ((list = parse()) != nullptr) {
    list->dump();
    cout << "\n";
  }
  return 0;
}
