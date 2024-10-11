#include "lexer.hpp"
#include "parser.hpp"
#include "runtime.hpp"

#include <fstream>
#include <iostream>
#include <sstream>


int main(int argc, char** argv) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << " path/to/file.ram" << std::endl;
    exit(1);
  }

  std::ifstream in(argv[2]);
  std::stringstream buf;
  buf << in.rdbuf();

  std::string program = R"(
input [;
x = 2;
a[0] = 1;
print a[0];
print ][;
[[ + 1 + ][] = 2;
print c + ][ + 1;
print [c + x + 1];
)";

  Lexer lexer(program);
  Parser parser(lexer);

  AST ast = parser.Parse();
  RunTime rt(ast);

  rt.Run();
  ast.Dump("graph.gv");

  return 0;

}