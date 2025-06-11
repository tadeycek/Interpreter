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

### Printing
If a variable is named lets say x:
```
let(x = 5)
print(x)
```
This would print the value of x, so `5`,
if there is no variable named x it would print `x`


### Conditional Statement
```
if (x > 3)
... print(x)
... end
```

Supported comparison operators: `==`, `!=`, `<`, `>`, `<=`, `>=`  

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
while(condition) 
... command1
... command2
... end
```
Example:
```
let (x=3)
while(x > 0) 
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
### Nested loops / if statements
```
while(condition) 
... if(condition)
... ... command2
... ... end
... end
```

### Exit the Program
```
exit
```
Or:
```
Control + c
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
- Defining and calling functions
- Return for functions
- Implementing math into conditions - so if and while
- Implementing lists
- Implementing arrays
- Implementing booleans
- Making a file version of the language
- Implementing the input, output functions for user input
- Adding other logical operators like && and ||
- Built in math functions like sqrt and pow
