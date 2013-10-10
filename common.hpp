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
typedef std::string String;
typedef std::unique_ptr<List> ListPtr;
// TODO Temporary work around. Codegen needed.
typedef String Code;
typedef Code (*CodeGenCallback)(const List& list);
void error(String s);
std::map<String, CodeGenCallback> callback;

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
  const std::vector<ListPtr>& get_children() const;

private:
  NodeType type;
  // union {
  String data;
  std::vector<ListPtr> children;
  // };
};
