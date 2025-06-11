# Simple C++ Interpreter

This is a basic command-line interpreter written in C++. It supports variables, math operations, conditional statements, and is currently being extended to include loops.

## How to Use

### Declare a Variable
```
let ( x = 5 )
```

### Update a Variable
```
let ( x = x - 3 )
```

### Print a Variable
```
print(x)
```

### Print Any Text or Value
```
print(asdad)
```

### Conditional Statement (Only `if` + `then` for now)
```
if (x > 3) then print(x)
```

Supported comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`  
Only `print`, `let`, and `math` commands are currently supported inside `if` blocks.

### Math Operations
```
math(3 + 8)
math(x / 4)
```

Supported math operations: `+`, `-`, `*`, `/`, `%`

### List All Variables
```
listVars()
```

Shows all variable names with their current values.

### While loop
```
while(condition) then
... command1
... command2
... end
```
Example:
```
let (x=3)
while(x > 0) then
...print(x)
...let(x = x - 1)
...end
```
Output
```
3
2
1
```



### Exit the Program
```
exit
```

Cleanly terminates the interpreter with `return 0`

## Features Completed

- [x] Math expression evaluation
- [x] Variable storage and updates
- [x] Conditional statements with `if`
- [x] Printing values or text
- [x] Variable listing
- [x] `while` loop (block-style)
- [x] Nested `while` loops and `if` statements, you can use one in another

## In Progress
- Nothing as of now, looking for more ideas.
