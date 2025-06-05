#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

unordered_map<string, string> variables;

string strip(string input) {
  string result;
  for (char c : input) {
    if (c != ' ') {
      result += c;
    }
  }
  return result;
}

string checkCommand(string input) {
  int index = input.find('(');
  string command = input.substr(0, index);
  command = strip(command);
  return command;
}

string otherPart(string input) {
  int index = input.find('(');
  int index2 = input.find(')');
  string output = input.substr(index + 1, index2 - index - 1);
  output = strip(output);
  return output;
}

bool addSub(string input, double &result) {
  string output = otherPart(input);
  output = strip(output);
  int eq = output.find('=');
  string nulPart;
  if (eq != string::npos) {
    nulPart = output.substr(0, eq - 1);
    output = output.substr(eq + 1);
  }
  int index;
  char op;
  if (output.find('+') != string::npos) {
    index = output.find('+');
    op = '+';
  } else if (output.find('-') != string::npos) {
    index = output.find('-');
    op = '-';
  } else if (output.find('*') != string::npos) {
    index = output.find('*');
    op = '*';
  } else if (output.find('/') != string::npos) {
    index = output.find('/');
    op = '/';
  } else
    return 0;
  string firstPart = output.substr(0, index);
  string secondPart = output.substr(index + 1);
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
    else if (op == '/')
      result = num2 != 0 ? num1 / num2 : 0.0;
    else
      return false;

    return true;

  } catch (...) {
    return false;
  }
}

bool handleLet(string input) {
  string remainder = otherPart(input);
  int eqPos = remainder.find('=');
  if (eqPos == string::npos)
    return false;
  string varName = remainder.substr(0, eqPos);
  string varValue = remainder.substr(eqPos + 1);

  double result;
  if (addSub(remainder, result))
    varValue = to_string(result);
  variables[varName] = varValue;
  return true;
}

void handlePrint(string input) {
  string val = otherPart(input);
  double result;
  if (addSub(input, result))
    cout << result << endl;

  else if (variables.count(val)) {
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

void handleIf(string input) {
  string remainder = otherPart(input);
  int thenPos = remainder.find("then");

  if (thenPos == string::npos) {
    cout << "Syntax error: missing 'then'" << endl;
    return;
  }

  string condition = remainder.substr(0, thenPos - 1);
  string action = remainder.substr(thenPos + 5);

  string opList[] = {"==", "!=", "<", ">", "<=", ">="};
  string opFound = "";
  int opPos = -1;

  for (string op : opList) {
    opPos = condition.find(op);
    if (opPos != string::npos) {
      opFound = op;
      break;
    }
  }

  if (opFound.empty()) {
    cout << "Unsupported or missing comparison operator" << endl;
    return;
  }

  string left = condition.substr(0, opPos);
  string right = condition.substr(opPos + opFound.length());

  while (!left.empty() && left[0] == ' ')
    left.erase(0, 1);
  while (!left.empty() && left.back() == ' ')
    left.pop_back();
  while (!right.empty() && right[0] == ' ')
    right.erase(0, 1);
  while (!right.empty() && right.back() == ' ')
    right.pop_back();

  string leftVal = variables.count(left) ? variables[left] : left;
  string rightVal = variables.count(right) ? variables[right] : right;

  bool conditionTrue = false;

  try {
    double num1 = stod(leftVal);
    double num2 = stod(rightVal);

    if (opFound == "==")
      conditionTrue = num1 == num2;
    else if (opFound == "!=")
      conditionTrue = num1 != num2;
    else if (opFound == ">")
      conditionTrue = num1 > num2;
    else if (opFound == "<")
      conditionTrue = num1 < num2;
    else if (opFound == ">=")
      conditionTrue = num1 >= num2;
    else if (opFound == "<=")
      conditionTrue = num1 <= num2;
  } catch (...) {

    if (opFound == "==")
      conditionTrue = leftVal == rightVal;
    else if (opFound == "!=")
      conditionTrue = leftVal != rightVal;
  }
  if (conditionTrue) {
    string subCommand = checkCommand(action);
    if (subCommand == "print")
      handlePrint(action);
    else
      cout << "Unknown action in 'then': " << subCommand << endl;
  }
}

int main() {
  string input;
  cout << "Language 1:\n";

  while (true) {
    cout << ">> ";
    if (!getline(cin, input))
      break;
    if (input == "exit")
      break;
    string command = checkCommand(input);
    if (command == "let") {
      if (!handleLet(input))
        cout << "Invalid variable syntax, use: let var = value" << endl;
    } else if (command == "print")
      handlePrint(input);
    else if (command == "math") {
      double result;
      if (addSub(input, result))
        cout << result << endl;
      else
        cout << "Wrong syntax or invalid math command.\n";
    } else if (command == "listVars") {
      if (variables.empty())
        cout << "No variables defined" << endl;
      else {
        for (auto v : variables)
          cout << v.first << " = " << v.second << endl;
      }
    } else if (command == "if")
      handleIf(input);

    else
      cout << "Unknown command\n";
  }
  cout << "Goodbye!\n";
  return 0;
}
