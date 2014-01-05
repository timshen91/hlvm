ListPtr tokenize() {
  static char ch = '\n';
  while (isspace(ch))
    ch = cin.get();
  if (cin.eof()) return nullptr;
  ListPtr list;
  switch (ch) {
    case '(':
      ch = cin.get();
      list = make_unique<List>(NodeType::list);
      while (ch != ')') {
        ensure(!cin.eof(), "Unexpected EOF");
        list->append(tokenize());
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
      list = make_unique<List>(data);
  }
  while (isspace(ch))
    ch = cin.get();
  return list;
}

ASTNodePtr parse(const List &list) {
  auto &children = list.get_children();
  ensure(children.size() > 0, "Keyword expected");
  ensure(handler.count(children[0]->get_string()) > 0,
         String("No such keyword: " + children[0]->get_string()));
  return handler[children[0]->get_string()](list);
}
