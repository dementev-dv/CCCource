

// Expr = io (var)
// OP = expr OP expr
// arr = base[offset]
// base = var
// offset = OP

class Stmt {


 private:
  Expr expr_;
};

class Expr {

};

#define skipspace(buf, len) while(isspace(buf[len])) len++
#define skipline(buf, len) while(buf[len] != '\n') len++

int GetOp(char *str, Expr **current) {
  int pos = 0;
  skipspace (str, pos);
  pos += GetArrRef (str + pos, current);
  skipspace (str, pos);

  while (str[pos] == '+' || str[pos] == '-') {
    *current->parent = new Expr;
    *current->parent->left = *current;
    *current = *current->parent;
    *current->op = str[pos];

    pos++;
    skipspace (str, pos);
    *current->right = GetArrRef (str + pos, current);
    skipspace (str, pos);
  }

  return pos;
}

