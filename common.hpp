// ---------- std ----------
namespace std {

template <typename T, typename... Args>
inline unique_ptr<T> make_unique(Args &&... args) {
  return unique_ptr<T>(new T(args...));
}

};

// ---------- declarations ----------
class List;
typedef string String;
typedef unique_ptr<List> ListPtr;

enum class NodeType {
  str, list,
};

class List {
 public:
  explicit List(NodeType type);
  explicit List(String data);
  void append(ListPtr &&t);
  void dump() const;

  const String &get_string() const;
  const vector<ListPtr> &get_children() const;

 private:
  NodeType type;
  // union {
  String data;
  vector<ListPtr> children;
  // };
};

class ASTNode {
 public:
  virtual void codegen() const = 0;
  virtual ~ASTNode() {}
};

// FIXME Will be reimplemented in the future.
typedef unique_ptr<ASTNode> ASTNodePtr;
typedef ASTNodePtr (*Handler)(const List &list);

// ---------- global definition ----------
extern map<String, Handler> handler;
map<String, set<String>> env_table = { { "file", { "#", "var", "function" } },
                                       { "function", { "#", "var", "if",
                                                       "return", "call" } }, };
vector<String> env_stack;

#define ensure(cond, msg)                                              \
  do {                                                                 \
    if (!(cond)) {                                                     \
      cerr << __FILE__ << " : " << __LINE__ << " : " << (msg) << "\n"; \
      exit(1);                                                         \
    }                                                                  \
  } while (false)
