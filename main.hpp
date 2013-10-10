#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <memory>
#include <map>
#include <string>
#include <vector>

namespace std {

template<typename T, typename... Args>
inline unique_ptr<T>
make_unique(Args&&... args) {
	return unique_ptr<T>(new T(args...));
}

};

enum class NodeType {
  str,
  list,
};

class List;
// FIXME Will be reimplemented in the future.
typedef std::string String;
typedef std::unique_ptr<List> ListPtr;
// TODO Temporary work around. Codegen needed.
typedef String Code;

typedef Code (*CodeGenCallback)(const List& list);

extern std::map<String, CodeGenCallback> callback;

void error(String s);

#endif
