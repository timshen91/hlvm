#include <iostream>
#include <memory>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <stack>
#include <tuple>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstddef>

using namespace std;

// ---------- Helpers ----------
#define ensure(cond, msg)                                              \
  do {                                                                 \
    if (!(cond)) {                                                     \
      cerr << __FILE__ << " : " << __LINE__ << " : " << (msg) << "\n"; \
      exit(1);                                                         \
    }                                                                  \
  } while (false)

// ---------- std ----------
namespace std {

template <typename T, typename... Args>
inline unique_ptr<T> make_unique(Args&&... args) {
  return unique_ptr<T>(new T(args...));
}
};

// ---------- AST ----------
class Node {
 public:
  virtual void codegen() const = 0;
  virtual ~Node() {}
};

// FIXME Will be reimplemented in the future.
typedef unique_ptr<Node> NodePtr;

class Environment;

// ---------- List ----------
class List;
typedef string String;
typedef unique_ptr<List> ListPtr;

enum class NodeType {
  symbol,
  list,
};

class List {
 public:
  NodeType type;
  // union {
  String data;
  vector<ListPtr> children;
  // };

  explicit List(NodeType type) : type(type) {}
  explicit List(String data) : type(NodeType::symbol), data(data) {}

  void append(ListPtr&& t) { children.push_back(move(t)); }

  const String& get_symbol() const {
    ensure(type == NodeType::symbol, "Symbol expected");
    return data;
  }

  size_t size() const {
    ensure(type == NodeType::list, "List expected");
    return children.size();
  }

  const List& operator[](size_t index) const {
    ensure(0 <= index && index < size(), "List out of bound");
    return *children[index];
  }

  void dump() const {
    static int depth = 0;
    print_depth(depth);
    switch (type) {
      case NodeType::list:
        cout << "(\n";
        depth++;
        for (auto& it : children) {
          it->dump();
          cout << "\n";
        }
        depth--;
        print_depth(depth);
        cout << ")";
        break;
      case NodeType::symbol:
        cout << data;
        break;
    }
  }

  void print_depth(int depth) const {
    while (depth--) cout << "  ";
  }
};

typedef NodePtr (*Handler)(Environment* env, const List& list);

// ---------- global definition ----------
extern map<String, tuple<String, String, Handler>> handler;

// ---------- parser ----------
ListPtr tokenize() {
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
  while (isspace(ch)) ch = cin.get();
  return list;
}

NodePtr parse(Environment* env, const List& list) {
  static vector<String> scope_stack = {"root_scope"};
  ensure(list.size() > 0, "Keyword expected");
  ensure(handler.count(list[0].get_symbol()) > 0,
         String("No such keyword: " + list[0].get_symbol()));
  const auto& h = handler[list[0].get_symbol()];
  ensure(get<0>(h) == "-" || scope_stack.back() == get<0>(h),
         "Pre scope not match");
  if (get<1>(h) != "-") {
    scope_stack.push_back(get<1>(h));
  } else {
    scope_stack.push_back(scope_stack.back());
  }
  auto ret = get<2>(h)(env, list);
  scope_stack.pop_back();
  return ret;
}

#include "core.cpp"

int main(int argc, char* args[]) {
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
  Environment top_env(nullptr);
  top_env.create_type(make_unique<Type>("int", 4));
  top_env.must_lookup_type("int");

  parse(&top_env, *list_root)->codegen();
  return 0;
}
