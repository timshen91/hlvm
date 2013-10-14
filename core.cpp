// TODO
ValuePtr handle_var(const List& list) {
  ensure(list.get_children().size() == 3, "");
  auto key = list.get_children()[1]->get_string();
  auto value = list.get_children()[2]->codegen();
  return nullptr;
}

// TODO
ValuePtr handle_function(const List& list) {
  ensure(list.get_children().size() == 3, "");
  auto pattern = list.get_children()[1]->codegen();
  auto body = list.get_children()[2]->codegen();
  return nullptr;
}

void init_core() {
  handler["var"] = handle_var;
  handler["function"] = handle_function;
}
