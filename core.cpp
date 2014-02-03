// ---------- file scope ----------
NodePtr handle_file(const List &list) {
  cerr << "handle_file\n";
  return nullptr;
}

// ---------- comment ----------
NodePtr handle_comment(const List &list) { return nullptr; }

// ---------- function ----------
NodePtr handle_function(const List &list) {
  cerr << "handle_function\n";
  return nullptr;
}

NodePtr handle_if(const List &list) {
  cerr << "handle_if\n";
  return nullptr;
}

NodePtr handle_lt(const List &list) {
  cerr << "handle_lt\n";
  return nullptr;
}

NodePtr handle_return(const List &list) {
  cerr << "handle_return\n";
  return nullptr;
}

NodePtr handle_plus(const List &list) {
  cerr << "handle_plus\n";
  return nullptr;
}

NodePtr handle_call(const List &list) {
  cerr << "handle_call\n";
  return nullptr;
}

NodePtr handle_minus(const List &list) {
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
