ListPtr parse(istream& in) {
  static char ch = '\n';
  while (isspace(ch)) ch = in.get();
  if (in.eof())
    return nullptr;
  if (ch == '(') {
    ch = in.get();
    auto list = make_unique<List>(NodeType::list);
    while (ch != ')') {
      ensure(!in.eof(), "Unexpected EOF");
      list->append(parse(in));
    }
    ch = in.get();
    return list;
  } else if (ch == ')') {
    ensure(false, "Unmatched ')'");
  } else {
    String data;
    while (!in.eof() && !isspace(ch) && ch != ')') {
      data += ch;
      ch = in.get();
    }
    return make_unique<List>(NodeType::str, data);
  }
  ensure(false, "Unreachable");
  return nullptr;
}

int main(int argc, char* args[]) {
  init_core();
  ListPtr list;
  if (argc >= 2) {
    ifstream fin(args[1]);
    list = parse(fin);
  } else {
    list = parse(cin);
  }
  ensure(list != nullptr, "");
  list->dump();
  cout << "\n\n";
  cout << list->codegen() << "\n";
  return 0;
}
