# [RAM

## Statements:
1) Присваивание: lvalue = expr;
2) Ввод: input lvalue;
3) Вывод: output expr;

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

Lvalue может быть 3 видов:
1) Имя переменной из латинских букв, например "value", либо дословно лексема "][" либо дословно лексема "["
2) Обращение к памяти вида name[expr], где name - имя состоящее из латинских букв, expr - любое допустимое выражение
3) Обращение к памяти вида [expr] где expr - любое допустимое выражение


# Результат работы парсера:

Для программы

```
input a;
a[0] = 1;
print a[0];
[[ + ][ + 1] = 1;
```

<image src="img/graph.png">

# Формулировка задания:

<image src="img/task.png">

