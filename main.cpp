#include <csignal>
#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

string strip(string input);
string trim(string input);
string checkCommand(string input);
string otherPart(string input);
bool addSub(string input, double &result);
bool handleLet(string input);
void handlePrint(string input);
bool evaluateCondition(const string &condition);
bool isValidCondition(const string &condition);
void executeCommands(const vector<string> &commands);
vector<string> readCommandBlock();
void handleSigint(int signum);

unordered_map<string, string> variables;

int main() {
  signal(SIGINT, handleSigint);

  string input;
  cout << "Language 1:\n";

  while (true) {
    cout << ">> ";
    if (!getline(cin, input))
      break;
    if (input == "exit")
      break;

    input = trim(input);
    if (input.empty())
      continue;

    string command = checkCommand(input);
    if (command == "let") {
      if (!handleLet(input))
        cout << "Invalid variable syntax, use: let(var = value)" << endl;
    } else if (command == "print") {
      handlePrint(input);
    } else if (command == "math") {
      double result;
      if (addSub(input, result))
        cout << result << endl;
      else
        cout << "Wrong syntax or invalid math command." << endl;
    } else if (command == "listVars") {
      if (variables.empty())
        cout << "No variables defined" << endl;
      else {
        for (const auto &v : variables)
          cout << v.first << " = " << v.second << endl;
      }
    } else if (command == "if" || command == "while") {
      // Validate condition syntax before proceeding
      string condition = otherPart(input);
      if (condition.empty()) {
        cout << "Syntax error: " << command << " statement requires a condition. Use: " << command << "(condition)" << endl;
        continue;
      }

      // Check if condition is valid
      if (!isValidCondition(condition)) {
        cout << "Syntax error: Invalid condition '" << condition << "'. Use comparison operators like ==, !=, <, >, <=, >=" << endl;
        continue;
      }

      vector<string> allCommands;
      allCommands.push_back(input);

      vector<string> blockCommands = readCommandBlock();
      allCommands.insert(allCommands.end(), blockCommands.begin(),
                         blockCommands.end());
      allCommands.push_back("end");

      executeCommands(allCommands);
    } else {
      // Fixed: Show the actual input when command is unknown or empty
      if (command.empty()) {
        cout << "Unknown command: " << input << " (missing parentheses?)" << endl;
      } else {
        cout << "Unknown command: " << command << endl;
      }
    }
  }
  cout << "Goodbye!\n";
  return 0;
}

string strip(string input) {
  string result;
  for (char c : input) {
    if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
      result += c;
    }
  }
  return result;
}

string trim(string input) {
  size_t start = input.find_first_not_of(" \t\n\r");
  if (start == string::npos)
    return "";
  size_t end = input.find_last_not_of(" \t\n\r");
  return input.substr(start, end - start + 1);
}

string checkCommand(string input) {
  size_t index = input.find('(');
  if (index == string::npos)
    return "";
  string command = input.substr(0, index);
  command = trim(command);
  return command;
}

string otherPart(string input) {
  size_t index = input.find('(');
  size_t index2 = input.find_last_of(')');
  if (index == string::npos || index2 == string::npos || index2 <= index) {
    return "";
  }
  string output = input.substr(index + 1, index2 - index - 1);
  output = trim(output);
  return output;
}

bool addSub(string input, double &result) {
  string output = otherPart(input);
  if (output.empty())
    return false;

  size_t eq = output.find('=');
  if (eq != string::npos) {
    output = output.substr(eq + 1);
    output = trim(output);
  }

  size_t index = string::npos;
  char op = 0;

  for (size_t i = 1; i < output.length(); i++) {
    if (output[i] == '+' || output[i] == '-') {
      index = i;
      op = output[i];
      break;
    }
  }

  if (index == string::npos) {
    for (size_t i = 1; i < output.length(); i++) {
      if (output[i] == '*' || output[i] == '/' || output[i] == '%') {
        index = i;
        op = output[i];
        break;
      }
    }
  }

  if (index == string::npos || op == 0)
    return false;

  string firstPart = trim(output.substr(0, index));
  string secondPart = trim(output.substr(index + 1));

  if (firstPart.empty() || secondPart.empty())
    return false;

  double num1, num2;
  try {
    if (variables.count(firstPart))
      num1 = stod(variables[firstPart]);
    else
      num1 = stod(firstPart);

    if (variables.count(secondPart))
      num2 = stod(variables[secondPart]);
    else
      num2 = stod(secondPart);

    if (op == '+')
      result = num1 + num2;
    else if (op == '-')
      result = num1 - num2;
    else if (op == '*')
      result = num1 * num2;
    else if (op == '%' && num2 != 0)
      result = (int)num1 % (int)num2;
    else if (op == '/') {
      if (num2 == 0) {
        cout << "Error: Division by zero" << endl;
        return false;
      }
      result = num1 / num2;
    } else
      return false;

    return true;

  } catch (...) {
    return false;
  }
}

bool handleLet(string input) {
  string remainder = otherPart(input);
  if (remainder.empty())
    return false;

  size_t eqPos = remainder.find('=');
  if (eqPos == string::npos)
    return false;

  string varName = trim(remainder.substr(0, eqPos));
  string varValue = trim(remainder.substr(eqPos + 1));

  if (varName.empty())
    return false;

  double result;
  if (addSub(input, result)) {
    variables[varName] = to_string(result);
  } else {
    variables[varName] = varValue;
  }
  return true;
}

void handlePrint(string input) {
  string val = otherPart(input);
  if (val.empty()) {
    cout << "" << endl;
    return;
  }

  double result;
  if (addSub(input, result)) {
    cout << result << endl;
  } else if (variables.count(val)) {
    string value = variables[val];
    try {
      double num = stod(value);
      if (num == (int)num)
        cout << (int)num << endl;
      else
        cout << num << endl;
    } catch (...) {
      cout << value << endl;
    }
  } else {
    cout << val << endl;
  }
}

bool isValidCondition(const string &condition) {
  if (condition.empty()) return false;

  // Check for valid comparison operators
  return (condition.find("==") != string::npos ||
          condition.find("!=") != string::npos ||
          condition.find("<=") != string::npos ||
          condition.find(">=") != string::npos ||
          condition.find("<") != string::npos ||
          condition.find(">") != string::npos);
}

bool evaluateCondition(const string &condition) {
  string op;
  size_t opPos = string::npos;

  if ((opPos = condition.find("==")) != string::npos) {
    op = "==";
  } else if ((opPos = condition.find("!=")) != string::npos) {
    op = "!=";
  } else if ((opPos = condition.find("<=")) != string::npos) {
    op = "<=";
  } else if ((opPos = condition.find(">=")) != string::npos) {
    op = ">=";
  } else if ((opPos = condition.find("<")) != string::npos) {
    op = "<";
  } else if ((opPos = condition.find(">")) != string::npos) {
    op = ">";
  }

  if (opPos == string::npos)
    return false;

  string left = trim(condition.substr(0, opPos));
  string right = trim(condition.substr(opPos + op.length()));

  string leftVal = variables.count(left) ? variables[left] : left;
  string rightVal = variables.count(right) ? variables[right] : right;

  try {
    double l = stod(leftVal), r = stod(rightVal);
    if (op == "==")
      return l == r;
    if (op == "!=")
      return l != r;
    if (op == "<=")
      return l <= r;
    if (op == ">=")
      return l >= r;
    if (op == "<")
      return l < r;
    if (op == ">")
      return l > r;
  } catch (...) {
    if (op == "==")
      return leftVal == rightVal;
    if (op == "!=")
      return leftVal != rightVal;
  }

  return false;
}

vector<string> readCommandBlock() {
  vector<string> commands;
  string userLine;
  int nestingLevel = 1;

  while (nestingLevel > 0) {
    cout << "... ";
    if (!getline(cin, userLine))
      break;
    userLine = trim(userLine);
    if (userLine.empty())
      continue;

    commands.push_back(userLine);

    if (checkCommand(userLine) == "if" || checkCommand(userLine) == "while")
      nestingLevel++;
    else if (userLine == "end")
      nestingLevel--;
  }

  return commands;
}

void executeCommands(const vector<string> &commands) {
  for (size_t i = 0; i < commands.size(); i++) {
    string cmd = commands[i];
    string command = checkCommand(cmd);

    if (command == "let") {
      if (!handleLet(cmd))
        cout << "Invalid variable syntax, use: let(var = value)" << endl;
    } else if (command == "print") {
      handlePrint(cmd);
    } else if (command == "math") {
      double result;
      if (addSub(cmd, result))
        cout << result << endl;
      else
        cout << "Wrong syntax or invalid math command." << endl;
    } else if (command == "if") {
      size_t condStart = cmd.find('(');
      size_t condEnd = cmd.find(')', condStart);

      if (condStart == string::npos || condEnd == string::npos) {
        cout << "Syntax error: missing brackets around condition" << endl;
        continue;
      }

      string condition =
          trim(cmd.substr(condStart + 1, condEnd - condStart - 1));

      // Validate condition
      if (condition.empty()) {
        cout << "Syntax error: if statement requires a condition. Use: if(condition)" << endl;
        continue;
      }

      if (!isValidCondition(condition)) {
        cout << "Syntax error: Invalid condition '" << condition << "'. Use comparison operators like ==, !=, <, >, <=, >=" << endl;
        continue;
      }

      int nestLevel = 1;
      size_t ifStart = i + 1;
      size_t ifEnd = i + 1;
      while (ifEnd < commands.size() && nestLevel > 0) {
        string checkCmd = checkCommand(commands[ifEnd]);
        if (checkCmd == "if" || checkCmd == "while") {
          nestLevel++;
        } else if (commands[ifEnd] == "end") {
          nestLevel--;
        }
        if (nestLevel > 0)
          ifEnd++;
      }
      if (evaluateCondition(condition)) {
        vector<string> ifBlock(commands.begin() + ifStart,
                               commands.begin() + ifEnd);
        executeCommands(ifBlock);
      }
      i = ifEnd;
    } else if (command == "while") {
      size_t condStart = cmd.find('(');
      size_t condEnd = cmd.find(')', condStart);

      if (condStart == string::npos || condEnd == string::npos) {
        cout << "Syntax error: missing brackets around condition" << endl;
        continue;
      }
      string condition =
          trim(cmd.substr(condStart + 1, condEnd - condStart - 1));

      // Validate condition
      if (condition.empty()) {
        cout << "Syntax error: while statement requires a condition. Use: while(condition)" << endl;
        continue;
      }

      if (!isValidCondition(condition)) {
        cout << "Syntax error: Invalid condition '" << condition << "'. Use comparison operators like ==, !=, <, >, <=, >=" << endl;
        continue;
      }

      int nestLevel = 1;
      size_t whileStart = i + 1;
      size_t whileEnd = i + 1;

      while (whileEnd < commands.size() && nestLevel > 0) {
        string checkCmd = checkCommand(commands[whileEnd]);
        if (checkCmd == "if" || checkCmd == "while") {
          nestLevel++;
        } else if (commands[whileEnd] == "end") {
          nestLevel--;
        }
        if (nestLevel > 0)
          whileEnd++;
      }
      while (evaluateCondition(condition)) {
        vector<string> whileBlock(commands.begin() + whileStart,
                                  commands.begin() + whileEnd);
        executeCommands(whileBlock);
      }
      i = whileEnd;
    } else if (cmd == "end")
      continue;
  }
}

void handleSigint(int signum) {
  cout << "\nInterrupt received (Ctrl+C). Exiting...\n";
  exit(signum);
}