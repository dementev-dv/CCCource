#ifndef AST_HPP_
#define AST_HPP_

class Node {
 public:
  Node(Node* p)
    : parent_(p) {
  }

 private:
  Node* parent_;
};

class ConstLeaf final : public Node {
 public:
  ConstLeaf(Node* p, unsigned v) 
    : Node(p),
      val_(v) {
  }
  
  unsigned val() { return val_; }

 private:
  unsigned val_;
};

class VarLeaf final : public Node {
 public:
  VarLeaf(Node* p, std::string n)
    : Node(p),
      name_(n) {
  }

  std::string name() { return name_; }

 private:
  std::string name_;
};

class Unar final : public Node {
 public:
  enum uType {
    INPUT,
    PRINT,
    MEMREF
  }

  Unar(Node* p, ioType t)
    : Node(p),
      type_(t) {
  }

 private:
  uType type_;
};

class Binar final : public Node {
 public:
  enum bType {
    PLUS,
    MINUS
  };

  Binar(Node* p, bType t)
    : Node(p),
      type_(t) {
  }

 private:
  bType type_;
};

#endif // AST_HPP_
