#ifndef AST_HPP_
#define AST_HPP_

class Node {
 public:
  Node(Node* p)
    : parent_(p) {
  }

  virtual void Dump();

  void SetParent(Node* parent) { parent_ = parent; }

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

  void Dump(std::ofstream out) {
    out << "peak" << this << "label[\"" << val_ << "\"]\n";
  }

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

  void Dump(std::ofstream out) {
    out << "peak" << this << "label[\"" << name_ << "\"]\n";
  }

 private:
  std::string name_;
};

class Unar final : public Node {
 public:
  enum uType {
    IN,
    OUT,
    REF
  }

  Unar(Node* p, ioType t)
    : Node(p),
      type_(t),
      op(NULL) {
  }

  ~Unar() {
    if (op) delete op;
  }

  void Dump(std::ofstream out) {
    out << "peak" << this << " [label = \"";
    switch(type_) {
      case IN:  out << "input";
      case OUT: out << "print";
      case REF: out << "REF";
    }
    out << "\"]\n"
    out << "peak" << this << " -> peak" << op << "\n";
    op->Dump();
  }

  Node* op;

 private:
  uType type_;
};

class Binar final : public Node {
 public:
  enum bType {
    ADD,        // Should we have the same names as in Token type enum??
    SUB,
    EQ,         // Should we have different classes for stmt and expr??
    SEQ
  };

  Binar(Node* p, bType t)
    : Node(p),
      type_(t),
      op1(NULL),
      op2(NULL) {
  }

  ~Binar() {
    if (op1) delete op1;
    if (op2) delete op2;
  }

  void Dump() {
    out << "peak" << this << " [label = \"";
    switch(type_) {
      case ADD: out << "+ ";
      case SUB: cout << "- ";
      case EQ:  out << "= ";
      case SEQ: out << "stmt sequence";
    }
    out << "\"]\n"
    out << "peak" << this << " -> peak" << op1 << "\n";
    out << "peak" << this << " -> peak" << op2 << "\n";
    op1->Dump();
    op2->Dump();
  }

  Node* op1;
  Node* op2;

 private:
  bType type_;
};

#endif // AST_HPP_
