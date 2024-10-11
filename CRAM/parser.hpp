#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "lexer.hpp"
#include "ast.hpp"

class Parser {
 public:
  Parser(Lexer& l)
    : lex_(l),
      curr_(START, "") {
  }

  AST Parse() {
    next();
    return AST(stmtseq(NULL));
  }

 private:
  Lexer& lex_;
  Token curr_;

  void expect(Tag tag) {
    if (curr_.tag() != tag) {
      std::cout << "Unexpected token " << curr_.str() << std::endl;
      std::cout << "Expected " << tag << std::endl;
      exit(1);      // Proper way to abort??
    }
  }

  void next() { curr_ = lex_.Next(); }

  Node* stmtseq(Node* parent) {
    Node* node = statement(NULL);
    if (curr_.tag() != END) {
      Binar* seq = new Binar(NULL, SEQ);
      node->SetParent(seq);
      seq->op1 = node;
      seq->op2 = stmtseq(seq);
      node = seq;
    }
    node->SetParent(parent);
    return node;
  }

  Node* statement(Node* parent) {
    switch(curr_.tag()) {
      case INPUT:
      {
        Unar* s = new Unar(parent, IN);
        next();
        s->op = lvalue(s);
        expect(SEMICOL);
        next();
        return s;
      }

      case PRINT:
      {
        Unar* s = new Unar(parent, OUT);
        next();
        s->op = expr(s);
        expect(SEMICOL);
        next();
        return s;
      }

      case END:
        std::cout << "Parsing stmt END " << std::endl;
        exit(1);
      case ERROR:
        std::cout << "Parsing stmt ERROR " << std::endl;
        exit(1);
      
      default:
      {
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
    }

    std::cout << "Unreachable" << std::endl;
    exit(1);
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
        std::cout << "Expected lvalue, but got " << curr_.str() << " token" << std::endl;
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
        Binar* bin = new Binar(NULL, ADD);
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
