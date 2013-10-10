#include "main.hpp"
#include "list.hpp"

#include <cassert>
#include <iostream>

using namespace std;

// TODO
Code codegen_val(const List& list) {
  assert(list.get_children().size() == 3);
  auto key = list.get_children()[1]->get_string();
  auto value = list.get_children()[2]->codegen();
  return Code("(") + "val" + " " + key + " " + value + ")";
}

// TODO
Code codegen_lambda(const List& list) {
  assert(list.get_children().size() == 3);
  auto pattern = list.get_children()[1]->codegen();
  auto body = list.get_children()[2]->codegen();
  return Code("(") + "lambda" + " " + pattern + " " + body + ")";
}

void init_core() {
  callback["val"] = codegen_val;
  callback["lambda"] = codegen_lambda;
}
