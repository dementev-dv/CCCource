#ifndef RUNTIME_HPP_
#define RUNTIME_HPP_

#include "parser.hpp"
#include "lexer.hpp"

#include <unordered_map>
#include <string>
#include <iterator>


class RunTime final {
  using VarTab = std::unordered_map<std::string, int>;
  using MemTab = std::unordered_map<int, int>;
  using VarTabIt = std::unordered_map<std::string, int>::iterator;
  using MemTabIt = std::unordered_map<int, int>::iterator;

 public:
  RunTime(Parser& parser) 
    : parser_(parser),
      ast_(parser_.Parse()) {
  }

  int Run() {
    Node* root = ast_.root();
    return Count(root);
  }

 private:
  VarTab vars_;
  MemTab mem_;
  Parser& parser_;
  AST& ast_;
  Node* root_;

  int SetLvalue(Node* node, int val) {
    if (VarLeaf* leaf = dynamic_cast<VarLeaf*>(node)) {
      VarTabIt it = vars_.find(leaf->name());
      if (it != vars_.end())
        vars_.erase(it);
      else
        std::cout << "Referencing unset variable " << leaf->name() << " returned 0 value" << std::endl; 
      vars_.emplace(leaf->name(), val);
      return val;
    }

    if (Unar* ref = dynamic_cast<Unar*>(node)) {
      if (ref->type() != REF) {
        std::cout << "Invalid op in input stmt" << std::endl;
        exit(1);
      }
      int addr = Count(ref->op);
      MemTabIt it = mem_.find(addr);
      if (it != mem_.end())
        mem_.erase(it);
      else
        std::cout << "Referencing unset memory addr [" << addr << "] returned 0 value" << std::endl; 
      mem_.emplace(addr, val);
      return val;
    }

    std::cout << "Expected lvalue, but got invalid AST node" << std::endl;
    exit(1);
  }

  int Count(Node* node) {
    if (VarLeaf* var = dynamic_cast<VarLeaf*>(node)) {
      VarTabIt it = vars_.find(var->name());
      if (it == vars_.end())
        return 0;
      return it->second;
    }

    if (ConstLeaf* cst = dynamic_cast<ConstLeaf*>(node)) {
      return cst->val();
    }

    if (Unar* un = dynamic_cast<Unar*>(node)) {
      Node* op = un->op;
      switch(un->type()) {
        case IN:
        {
          int val;
          std::cin >> val;
          SetLvalue(op, val);
        }
        
        case OUT:
        {
          int val = Count(op);
          std::cout << val << std::endl;
          return val;
        }

        case REF:
        {
          int addr = Count(op);
          MemTabIt it = mem_.find(addr);
          if (it == mem_.end()) {
            mem_.emplace(addr, 0);
            return 0;
          }
          return it->second;
        }

        default:
          std::cout << "Unknown unar AST node" << std::endl;
          exit(1);
      }
    }

    if (Binar* bin = dynamic_cast<Binar*>(node)) {
      Node* op1 = bin->op1;
      Node* op2 = bin->op2;

      switch(bin->type()) {
        case ADD:
          return Count(op1) + Count(op2);
        case SUB:
          return Count(op1) - Count(op2);

        case EQ:
        {
          int val = Count(op2);
          SetLvalue(op1, val);
          return val;
        }

        case SEQ:
        {
          Count(op1);
          int val = Count(op2);
          return val;
        }

        default:
          std::cout << "Unknown binar AST node" << std::endl;
          exit(1);
      }
    }

    std::cout << "Unreachable1" << std::endl;
    exit(1);
  }
};

#endif // RUNTIME_HPP_
