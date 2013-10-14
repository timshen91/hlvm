// TODO
Code handle_val(const List& list) {
  ensure(list.get_children().size() == 3, "");
  auto key = list.get_children()[1]->get_string();
  auto value = list.get_children()[2]->codegen();
  return Code("(") + "val" + " " + key + " " + value + ")";
}

// TODO
Code handle_lambda(const List& list) {
  ensure(list.get_children().size() == 3, "");
  auto pattern = list.get_children()[1]->codegen();
  auto body = list.get_children()[2]->codegen();
  return Code("(") + "lambda" + " " + pattern + " " + body + ")";
}

void init_core() {
  handler["val"] = handle_val;
  handler["lambda"] = handle_lambda;
}
