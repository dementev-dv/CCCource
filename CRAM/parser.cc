#include <iostream>
#include <fstream>

class Program {
 public:
  
  Program() = delete
  Program(std::string path);
  ~Program();
  void Parse();

 private:
  std::string path_;
  std::ifstream ifile_;
  std::vector<Stmt*> stmt_;
};

void Program::Parse () {
  char c;
  int curr = 0;
  int len = 0;
  std::string stmt;
  int line = 0;

  while (!ifile_.eof()) {
    ifile_ >> c;
    if (c == '\n')
      line ++;
    else {                                // c != newline
      stmt[len] = 'c';
      len ++;
    }
    if (c == ';') {                       // end of statement
      stmt[len] = '\0';
      stmt_[curr] = new Stmt(stmt, line);
      stmt_[curr].Parse();
      curr ++;
      len = 0;
      stmt.clear();
    }
  }

  if (c != ';') {                         // last statement does not end with ';' symbol
    std::cout << path_ << ":" << line << ":" << len << "statement parsing error:" << std::endl;
    std::cout << stmt << std::endl;
    for (int i = 0; i < len-1; i ++)
      ctd::cout << ' ';
    std::cout << '^' << std::endl;
    std::cout << "Expected \';\' symbol before EOF taken" << std::endl;
  }

  return;
}

class Stmt {
 public:
  enum StmtType {
    IO,
    ASSIGN
  }

  Stmt();
  Stmt(std::string str, int line);
  ~Stmt();


  void Parse(char *buf);

 private:
  enum StmtType type_;
  Expr *entry_;
  std::string str_;
  int line;
};

#define skipspace(buf, len) while(isspace(buf[len])) len++

void Stmt::Parse () {
  std::string expr;
  int pos = 0;
  int len = 0;
  
  skipspace(str_, pos);

  if (str_[pos] == ';') {
    entry = new Expr(Expr::NOP);
    return;
  }

  if (str_[pos] == '[') {
    entry = GetAssign(this);
    return;
  }

  while (isalpha(str_[pos])) {
    expr[len] = str_[pos];
    pos ++;
    len ++;
  }
  expr[len] = '\0';

  if (expr == "input" || expr == "print") {
    entry_ = GetIO(this);
    return;
  }

  entry = GetAssign(this);
  return;
}

class Expr {
 public:
  enum ExprType {
    IO,
    MEM,
    VAR,
    OP,
    NOP,
    UNKNWN
  };

  Expr();
  ~Expr();


 private:
  Stmt *stmt_;
  Expr *parent_;
  Expr *left_;
  Expr *right_;
  enum ExprType type_;
  std::string str_;
};
