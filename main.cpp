int main(int argc, char *args[]) {
  handler["file"] = handle_file;
  handler["#"] = handle_comment;
  //handler["struct"] = handle_struct;
  //handler["int"] = handle_int;
  //handler["var"] = handle_var;
  //handler["="] = handle_assign;
  handler["function"] = handle_function;
  handler["if"] = handle_if;
  handler["<="] = handle_lt;
  handler["return"] = handle_return;
  handler["+"] = handle_plus;
  handler["call"] = handle_call;
  handler["-"] = handle_minus;

  if (argc >= 2 && strcmp("-", args[1]) != 0) {
    freopen(args[1], "r", stdin);
    if (argc >= 3 && strcmp("-", args[2]) != 0) {
      freopen(args[2], "w", stdout);
    }
  }

  auto list_root = make_unique<List>(NodeType::list);
  list_root->append(make_unique<List>("file"));

  ListPtr list;
  while ((list = tokenize()) != nullptr) {
    list_root->append(move(list));
  }
  auto ast_root = parse(*list_root);
  ast_root->codegen();
  return 0;
}
