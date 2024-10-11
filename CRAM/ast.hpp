#ifndef AST_HPP_
#define AST_HPP_

#include <fstream>

class Node {
 public:
  Node(Node* p)
    : parent_(p) {
  }

  virtual void Dump(std::ofstream&) { }

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

  void Dump(std::ofstream& out) {
    out << "\tpeak" << this << " [label = \"" << val_ << "\"]\n";
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

  void Dump(std::ofstream& out) {
    out << "\tpeak" << this << " [label = \"" << name_ << "\"]\n";
  }

 private:
  std::string name_;
};

enum uType {
  IN,
  OUT,
  REF
};

class Unar final : public Node {
 public:
  Unar(Node* p, uType t)
    : Node(p),
      type_(t),
      op(NULL) {
  }

  ~Unar() {
    if (op) delete op;
  }

  uType type() { return type_; }

  void Dump(std::ofstream& out) {
    out << "\tpeak" << this << " [label = \"";
    switch(type_) {
      case IN:  out << "input"; break;
      case OUT: out << "print"; break;
      case REF: out << "REF"; break;
      default:  out << "UNKNOWN"; break;
    }
    out << "\"]\n";
    out << "\t\tpeak" << this << " -> peak" << op << "\n";
    op->Dump(out);
  }

  Node* op;

 private:
  uType type_;
};

enum bType {
  ADD,        // Should we have the same names as in Token type enum??
  SUB,
  EQ,         // Should we have different classes for stmt and expr??
  SEQ // sequence of statements
};

class Binar final : public Node {
 public:
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

  bType type() { return type_; }

  void Dump(std::ofstream& out) {
    out << "\tpeak" << this << " [label = \"";
    switch(type_) {
      case ADD: out << "+"; break;
      case SUB: out << "-"; break;
      case EQ:  out << "="; break;
      case SEQ: out << "stmt sequence"; break;
      default:  out << "unknown"; break;
    }
    out << "\"]\n";
    out << "\t\tpeak" << this << " -> peak" << op1 << "\n";
    out << "\t\tpeak" << this << " -> peak" << op2 << "\n";
    op1->Dump(out);
    op2->Dump(out);
  }

  Node* op1;
  Node* op2;

 private:
  bType type_;
};

struct AST {
  AST(Node* root, unsigned d)
    : root_(root),
      depth_(d) {
  }

  ~AST() {
    delete root_;
  }

  Node* root() { return root_; }

  void SetRoot(Node* root) { root_ = root; }
  void SetDepth(unsigned d) { depth_ = d; }

 private:
  Node* root_;
  unsigned depth_;
};

#endif // AST_HPP_
