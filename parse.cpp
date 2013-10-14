ListPtr parse() {
  static char ch = '\n';
  while (isspace(ch)) ch = cin.get();
  if (cin.eof()) return nullptr;
  ListPtr list;
  switch (ch) {
  case '(':
    ch = cin.get();
    list = make_unique<List>(NodeType::list);
    while (ch != ')') {
      ensure(!cin.eof(), "Unexpected EOF");
      list->append(parse());
    }
    ch = cin.get();
    break;
  case ')':
    ensure(false, "Unmatched ')'");
    break;
  default:
    String data;
    while (!cin.eof() && !isspace(ch) && ch != ')') {
      if (ch == '\\') {
        ch = cin.get();
        if (!cin.eof()) {
          data += ch;
        } else {
          data += '\\';
        }
      } else {
        data += ch;
      }
      ch = cin.get();
    }
    list = make_unique<List>(NodeType::str, data);
  }
  while (isspace(ch)) ch = cin.get();
  return list;
}
