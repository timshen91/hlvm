ValuePtr handle_comment(const List &list) { return nullptr; }

// TODO
ValuePtr handle_struct(const List &list) {
  cerr << "handle_struct\n";
  return nullptr;
}

// TODO
ValuePtr handle_assign(const List &list) {
  cerr << "handle_assign\n";
  return nullptr;
}

// TODO
ValuePtr handle_var(const List &list) {
  cerr << "handle_var\n";
  return nullptr;
}

// TODO
ValuePtr handle_function(const List &list) {
  cerr << "handle_function\n";
  return nullptr;
}

// TODO
ValuePtr handle_call(const List &list) {
  cerr << "handle_call\n";
  return nullptr;
}

void init_core() {
  handler["#"] = handle_comment;
  handler["var"] = handle_var;
  handler["function"] = handle_function;
  handler["struct"] = handle_struct;
  handler["assign"] = handle_assign;
  handler["call"] = handle_call;
}
