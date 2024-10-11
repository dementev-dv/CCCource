#include "lexer.hpp"
#include "parser.hpp"

#include <fstream>

int main() {
  std::string program = R"(
    input a;
    a[0] = 1;
    print a[0];
    [[ + ][ + 1] = 1;
  )";

  Lexer lexer(program);
  Parser parser(lexer);
  
  parser.Parse();
  parser.Dump("graph.gv");
  return 0;

}