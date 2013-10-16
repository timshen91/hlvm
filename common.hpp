// ---------- std ----------
namespace std {

template <typename T, typename... Args>
inline unique_ptr<T> make_unique(Args &&... args) {
  return unique_ptr<T>(new T(args...));
}
};

// ---------- declaration ----------
class List;
// FIXME Will be reimplemented in the future.
typedef string String;
typedef unique_ptr<List> ListPtr;
typedef unique_ptr<llvm::Value> ValuePtr;
typedef ValuePtr (*Handler)(const List &list);

// ---------- detailed declaration ----------
enum class NodeType {
  str,
  list,
};

class List {
 public:
  List(NodeType type);
  explicit List(NodeType type, String data);
  void append(ListPtr &&t);
  void dump() const;
  ValuePtr codegen() const;

  const String &get_string() const;
  const vector<ListPtr> &get_children() const;

 private:
  NodeType type;
  // union {
  String data;
  vector<ListPtr> children;
  // };
};

// ---------- global definition ----------
map<String, Handler> handler;

#define ensure(cond, msg)                                              \
  do {                                                                 \
    if (!(cond)) {                                                     \
      cerr << __FILE__ << " : " << __LINE__ << " : " << (msg) << "\n"; \
      exit(1);                                                         \
    }                                                                  \
  } while (false)
