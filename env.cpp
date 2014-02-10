class Type;
class Variable;
class FunctionNode;

// All envs form a tree; every env stores variables, functions and types, so
// that it enables defining function in a function scope, or even in a for loop.
// Defining types in non-top-env is also possible.
class Environment {
 public:
  map<String, unique_ptr<Variable>> vars;
  map<String, unique_ptr<Type>> types;
  map<String, unique_ptr<FunctionNode>> functions;
  Environment* parent;

  Environment(Environment* parent) : parent(parent) {}

  Variable* create_var(unique_ptr<Variable>&& ptr) {
    return create_impl(vars, move(ptr));
  }

  FunctionNode* create_func(unique_ptr<FunctionNode>&& ptr) {
    return create_impl(functions, move(ptr));
  }

  Type* create_type(unique_ptr<Type>&& ptr) {
    return create_impl(types, move(ptr));
  }

  Variable* must_lookup_var(String name) const {
    return must_lookup<Variable>(offsetof(Environment, vars), name);
  }

  FunctionNode* must_lookup_func(String name) const {
    return must_lookup<FunctionNode>(offsetof(Environment, functions), name);
  }

  Type* must_lookup_type(String name) const {
    return must_lookup<Type>(offsetof(Environment, types), name);
  }

 private:
  template <typename T>
  T* create_impl(map<String, unique_ptr<T>>& c, unique_ptr<T>&& ptr) {
    ensure(c.count(ptr->name) == 0, "Duplicated definition");
    return &*(c[ptr->name] = move(ptr));
  }

  template <typename T>
  T* must_lookup(size_t member_offset, String name) const {
    ensure(this, String("`" + name + "` is not defined"));
    const auto& c = *static_cast<map<String, unique_ptr<T>>*>(
                         (void*)((char*)this + member_offset));
    if (c.count(name) == 0) {
      return parent->must_lookup<T>(member_offset, name);
    }
    return c.at(name).get();
  }
};

Environment top_env(nullptr);
vector<Environment*> env_stack = {&top_env};
