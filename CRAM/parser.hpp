#include "lexer.hpp"

#include <stack>

class State {
 public:
  enum Element {
    STMT,
    EXPR,
    LVAL,
    ASSIGN,
    PLUS,
    MINUS,
    VAL,
    
  }

  State() {
    stk_.push
  }

 private:
  std::stack<> stk_;
};

class Parser {
 public:
  Parser(Lexer& l)
    : lex_(l),
      root_(NULL),
      curr_(START, "") {
  }

  void Parse() {

  }


 private:
  Lexer& lex_;
  Node* root_;
  Token curr_;

  void expect(Tag tag) {
    Token t = lex_.Next();
    if (t.tag() != tag) {
      std::cout << "Unexpected token " << t.str() << " taken after " curr_.str() << std::endl;
      exit(1);      // Proper way to abort??
    }
    curr_ = t;
  }

  void next() { curr_ = lex_.Next(); }

  Node* statement(Node* parent) {
    if (curr_.tag() == INPUT) {
      Unar* s = new Unar(IN, parent);
      next();
      s->op = lvalue(s);
      expect(SEMICOL);
      next();
      return s;
    }

    if (curr_.tag() == PRINT) {
      Unar* s = new Unar(OUT, parent);
      next();
      s->op = expr(s);
      expect(SEMICOL);
      next();
      return s;
    }

    // Assignment statement or error
    Binar* s = new Binar(EQ, parent);
    next();
    s->op1 = lvalue(s);
    expect(ASSIGN);
    next();
    s->op2 = expr(s);
    expect(SEMICOL);
    next();
    return s;
  }

  Node* lvalue(Node* parent) {
    if (curr_.tag() == LBRACK) {
      Node* val = new Unar(REF, parent);
      next();
      val->op = expr();
      next();
      return val;
    }

    if (curr_.tag() != ID) {
      std::cout << "Expected lvalue stmt, but got " << curr_.str() << " token" << std::endl;
      exit(1);
    }

    Token prev = curr_;
    next();



  }

  Node* val(Node* parent) {
    Node* node;
    switch(curr_.tag()) {
      case NUM:
        node = return ConstLeaf(parent, stoi(curr_.str()));
        next();
        return node;

      case ID:
        node = new VarLeaf(NULL, curr_.str());
        next();
        if (curr_.tag() == LBRACK) {
          Node* tmp = node;
          node = new Unar(parent, REF);
          node->op = new Binar(node, ADD);
          node->op->op1 = tmp;
          tmp->SetParent(node->op);
          next();
          node->op->op2 = expr();
          expect(RBRACK);
          return node;
        } 
        node->SetParent(parent);
        return node;

      case LBRACK:  
        node = new Unar(parent, REF);
        next();
        node->op = expr();
        return node;
      default: 
        std::cout << "Expected value, but got " << curr_.str() << " token" << std::endl;
        exit(1);
    }
  }

  Node* expr() {
    next();
    Node* node = val();
    next();

    while(curr_.tag() == MINUS || curr_.tag() == PLUS) {
      if (curr_.tag() == MINUS) {
        Node* tmp = node;
        node = new Binar(NULL, SUB);
        node->op1 = tmp;
        tmp->SetParent(node);
        next();
        node->op2 = val(node);
      }
      if (curr_.tag() == PLUS) {
        Node* tmp = node;
        node = new Binar(NULL, SUB);
        node->op1 = tmp;
        tmp->SetParent(node);
        next();
        node->op2 = val(node);
      }
    }

    return node;
  }

};
