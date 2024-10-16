# [RAM

## Make and run

```shell
cmake . -B build
cmake --build build -j

# Write your code to input.ram file

./build/ram input.ram
```

## Statements:

```EBNF
AssignStmt -> lvalue = expr;
InputStmt -> input lvalue;
PrintStmt -> print expr;
Stmt -> AssignStmt | InputStmt | PrintStmt
StmtSequence -> Stmt StmtSequence | Stmt
```

## Expressions:
```EBNF
digit -> 0|1| ... |9
num -> digit digit*
letter -> a|b| ... |z|A|B ... |Z
id -> (letter letter*) | \[ | \]\[
mem -> \[ expr \]
array -> id \[ expr \]
lvalue -> id | array | mem
value -> lvalue | num
expr -> expr + value | expr - value | value
```


# Результат работы парсера:

Для программы

```
input [;
x = 2;
a[0] = 1;
print a[0];
print ][;
[[ + 1 + ][] = 2;
print c + ][ + 1;
print [c + x + 1];
```

<image src="img/graph.png">

# Формулировка задания:

<image src="img/task.png">
