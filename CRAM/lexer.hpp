#include <iostream>
#include <string>
#include <vector>


enum Tag {
  ID,
  NUM,
  LBRACK,
  RBRACK,
  SEMICOL,
  INPUT,
  OUTPUT,
  EQ,
  ERROR,
  END,
  PLUS,
  MINUS
};

class Token {
 public:
  Token(Tag t, std::string v)
    : tag_(t),
      val_(v) {
    std::cout << "Token " << v << std::endl;
  }

  Tag tag() { return tag_; }

 private:
  Tag tag_;
  std::string val_;
};

class Lexer {
 public:
  Lexer(std::string input)
    : in_(input),
      arr_(false),
      arrcnt_(0),
      pos_(0) {
    std::cout << "Lexer " << input << std::endl;
  }

  Token Next() {
    while(pos_ < in_.size() && isspace(in_[pos_])) {
      pos_++;
    }

    if (pos_ >= in_.size()) {
      return Token(END, "");
    }

    char curr = in_[pos_];

    if (isdigit(curr)) {
      std::string num;
      while (pos_ < in_.size() && isdigit(in_[pos_])) {
        num += in_[pos_];
        pos_++;
      }
      return Token(NUM, num);
    }

    if (is_val_p(curr)) {
      std::string id;
      if (isalpha(curr)) {
        while(pos_ < in_.size() && isalpha(in_[pos_])) {
          id += in_[pos_];
          pos_++;
        }
        if(in_[pos_] == '[')
          arr_ = true;
        return Token(ID, id);
      }

      if (curr == '[') {
        char next = in_[++pos_];
        if (arr_) {
          arr_ = false;
          arrcnt_++;
          return Token(LBRACK, "[");
        }
        if (is_val_p(next) || isdigit(next))
          return Token(LBRACK, "[");
        return Token(ID, "c");
      }

      if (curr == ']') {
        char next = in_[++pos_];
        if (arrcnt_) { --arrcnt_; return Token(RBRACK, "]"); }
        if (next == '[') {
          ++pos_;
          return Token(ID, "x");
        }
        return Token(RBRACK, "]");
      }
    }

    ++pos_;
    switch(curr) {
      case '=': return Token(EQ, "=");
      case '+': return Token(PLUS, "+");
      case '-': return Token(MINUS, "-");
      case ';': return Token(SEMICOL, ";");
      case '[': return Token(LBRACK, "[");
      case ']': return Token(RBRACK, "]");
      default: return Token(ERROR, std::string(1, curr));
    }
  }

 private:
  bool is_val_p(char c) {
    if (isalpha(c) || c == '[' || c == ']')
      return true;
    return false;
  }

  bool arr_;
  std::string in_;
  unsigned pos_;
  unsigned arrcnt_;
};
