int main(int argc, char *args[]) {
  init_core();
  if (argc >= 2) {
    freopen(args[1], "r", stdin);
  }
  ListPtr list;
  while ((list = parse()) != nullptr) {
    list->codegen();
  }
  return 0;
}
