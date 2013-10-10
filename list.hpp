#ifndef LIST_HPP_
#define LIST_HPP_

#include "main.hpp"

class List {
public:
  List(NodeType type);
  explicit List(NodeType type, String data);
  void append(ListPtr&& t);
  void dump() const;
  Code codegen() const;

  const String& get_string() const;
  const std::vector<ListPtr>& get_children() const;

private:
  NodeType type;
  // union {
  String data;
  std::vector<ListPtr> children;
  // };
};

#endif
