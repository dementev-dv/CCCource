#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "lexer.hpp"
#include "ast.hpp"

class Parser {
 public:
  Parser(Lexer& l)
    : lex_(l),
      root_(NULL),
      curr_(START, "") {
  }

  void Parse() {
    next();
    Node* node = statement(NULL);
    while(curr_.tag() != END) {
      Node* tmp = node;
      Binar* bin = new Binar(NULL, SEQ);
      tmp->SetParent(node);
      bin->op1 = tmp;
      bin->op2 = statement(node);
      node = bin;
    }

    root_ = node;
  }

  void Dump(const char* path) {
    if (!root_) return;
    std::ofstream out(path);
    out << "digraph G {\n";
    if (Unar* r = dynamic_cast<Unar*>(root_)) r->Dump(out);
    if (Binar* r = dynamic_cast<Binar*>(root_)) r->Dump(out);
    if (ConstLeaf* r = dynamic_cast<ConstLeaf*>(root_)) r->Dump(out);
    if (VarLeaf* r = dynamic_cast<VarLeaf*>(root_)) r->Dump(out);
    out << "}\n";
    out.close();
  }


 private:
  Lexer& lex_;
  Node* root_;
  Token curr_;

  void expect(Tag tag) {
    if (curr_.tag() != tag) {
      std::cout << "Unexpected token " << curr_.str() << std::endl;
      exit(1);      // Proper way to abort??
    }
  }

  void next() { curr_ = lex_.Next(); }

  Node* statement(Node* parent) {
    if (curr_.tag() == INPUT) {
      Unar* s = new Unar(parent, IN);
      next();
      s->op = lvalue(s);
      expect(SEMICOL);
      next();
      return s;
    }

    if (curr_.tag() == PRINT) {
      Unar* s = new Unar(parent, OUT);
      next();
      s->op = expr(s);
      expect(SEMICOL);
      next();
      return s;
    }

    // Assignment statement or error
    Binar* s = new Binar(parent, EQ);
    s->op1 = lvalue(s);
    expect(ASSIGN);
    next();
    s->op2 = expr(s);
    expect(SEMICOL);
    next();
    return s;
  }

  Node* lvalue(Node* parent) {
    switch(curr_.tag()) {
      case ID:
      {
        Node* node = new VarLeaf(NULL, curr_.str());
        next();
        if (curr_.tag() == LBRACK) {
          Node* tmp = node;
          Unar* un = new Unar(parent, REF);
          Binar* bin = new Binar(node, ADD);
          un->op = bin;
          bin->op1 = tmp;
          tmp->SetParent(un->op);
          next();
          bin->op2 = expr(un->op);
          expect(RBRACK);
          next();
          node = un;
        } 
        node->SetParent(parent);
        return node;
      }

      case LBRACK:
      {
        Unar* un = new Unar(parent, REF);
        next();
        un->op = expr(un);
        expect(RBRACK);
        next();
        return un;
      }

      default:
        std::cout << "Expected value, but got " << curr_.str() << " token" << std::endl;
        exit(1);
    }
    return NULL;
  }

  Node* val(Node* parent) {
    Node* node;
    switch(curr_.tag()) {
      case NUM:
        node = new ConstLeaf(parent, stoi(curr_.str()));
        next();
        return node;

      case ID:
      case LBRACK:
        return lvalue(parent);

      default:
        std::cout << "Expected value, but got " << curr_.str() << " token" << std::endl;
        exit(1);
    }
  }

  Node* expr(Node* parent) {
    Node* node = val(NULL);

    while(curr_.tag() == MINUS || curr_.tag() == PLUS) {
      if (curr_.tag() == MINUS) {
        Node* tmp = node;
        Binar* bin = new Binar(NULL, SUB);
        bin->op1 = tmp;
        tmp->SetParent(bin);
        next();
        bin->op2 = val(bin);
        node = bin;
      }
      if (curr_.tag() == PLUS) {
        Node* tmp = node;
        Binar* bin = new Binar(NULL, SUB);
        bin->op1 = tmp;
        tmp->SetParent(bin);
        next();
        bin->op2 = val(bin);
        node = bin;
      }
    }

    node->SetParent(parent);
    return node;
  }

};

#endif // PARSER_HPP_
