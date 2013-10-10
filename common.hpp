// ---------- std ----------
namespace std {

template<typename T, typename... Args>
inline unique_ptr<T>
make_unique(Args&&... args) {
	return unique_ptr<T>(new T(args...));
}

};

// ---------- declare ----------
class List;
// FIXME Will be reimplemented in the future.
typedef string String;
typedef unique_ptr<List> ListPtr;
// TODO Temporary work around. Codegen needed.
typedef String Code;
typedef Code (*CodeGenCallback)(const List& list);

// ---------- detailed declare ----------
enum class NodeType {
  str,
  list,
};

class List {
public:
  List(NodeType type);
  explicit List(NodeType type, String data);
  void append(ListPtr&& t);
  void dump() const;
  Code codegen() const;

  const String& get_string() const;
  const vector<ListPtr>& get_children() const;

private:
  NodeType type;
  // union {
  String data;
  vector<ListPtr> children;
  // };
};

// ---------- global definition ----------
map<String, CodeGenCallback> callback;

void error(String s) {
  cerr << s << "\n";
  exit(1);
}

void ensure(bool cond, String msg) {
  if (!cond) {
    error(msg);
  }
}