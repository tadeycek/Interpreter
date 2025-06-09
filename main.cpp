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
void handleIf(string input);
bool evaluateCondition(const string &condition);
void executeCommand(const string &cmd);
void handleWhile(string input);

unordered_map<string, string> variables;

int main() {
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
	} else if (command == "if") {
	  handleIf(input);
	} else if (command == "while") {
	  handleWhile(input);
	} else {
	  cout << "Unknown command: " << command << endl;
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
	else if(op == '%' && num2 != 0)
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

void handleIf(string input) {
  size_t condStart = input.find('(');
  size_t condEnd = input.find(')', condStart);

  if (condStart == string::npos || condEnd == string::npos) {
	cout << "Syntax error: missing brackets around condition" << endl;
	return;
  }

  string condition = trim(input.substr(condStart + 1, condEnd - condStart - 1));
  string remainder = trim(input.substr(condEnd + 1));

  if (condition.empty()) {
	cout << "Syntax error: empty condition" << endl;
	return;
  }

  if (remainder.length() < 4 || remainder.substr(0, 4) != "then") {
	cout << "Syntax error: missing 'then' after condition" << endl;
	return;
  }

  vector<string> blockCommands;
  string userLine;

  cout << "Enter commands (type 'end' to finish if block):" << endl;
  while (true) {
	cout << "... ";
	if (!getline(cin, userLine))
	  break;
	userLine = trim(userLine);
	if (userLine == "end")
	  break;
	if (!userLine.empty())
	  blockCommands.push_back(userLine);
  }

  if (evaluateCondition(condition)) {
	for (const string &cmd : blockCommands) {
	  executeCommand(cmd);
	}
  }
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

  if (opPos == string::npos) {
	cout << "Syntax error: supported operators are ==, !=, <=, >=, <, >"
		 << endl;
	return false;
  }

  string left = trim(condition.substr(0, opPos));
  string right = trim(condition.substr(opPos + op.length()));

  if (left.empty() || right.empty()) {
	cout << "Invalid condition syntax" << endl;
	return false;
  }

  string leftVal = variables.count(left) ? variables[left] : left;
  string rightVal = variables.count(right) ? variables[right] : right;

  bool conditionTrue = false;

  try {
	double num1 = stod(leftVal);
	double num2 = stod(rightVal);

	if (op == "==") {
	  conditionTrue = num1 == num2;
	} else if (op == "!=") {
	  conditionTrue = num1 != num2;
	} else if (op == "<=") {
	  conditionTrue = num1 <= num2;
	} else if (op == ">=") {
	  conditionTrue = num1 >= num2;
	} else if (op == "<") {
	  conditionTrue = num1 < num2;
	} else if (op == ">") {
	  conditionTrue = num1 > num2;
	}
  } catch (...) {
	if (op == "==") {
	  conditionTrue = leftVal == rightVal;
	} else if (op == "!=") {
	  conditionTrue = leftVal != rightVal;
	} else {
	  cout << "Error: Numerical comparison operators (<, >, <=, >=) require "
			  "numbers"
		   << endl;
	  return false;
	}
  }

  return conditionTrue;
}

void executeCommand(const string &cmd) {
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
	handleIf(cmd);
  } else {
	cout << "Unknown command in while loop: " << command << endl;
  }
}

void handleWhile(string input) {
  size_t condStart = input.find('(');
  size_t condEnd = input.find(')', condStart);

  if (condStart == string::npos || condEnd == string::npos) {
	cout << "Syntax error: missing brackets around condition" << endl;
	return;
  }

  string condition = trim(input.substr(condStart + 1, condEnd - condStart - 1));
  string remainder = trim(input.substr(condEnd + 1));

  if (condition.empty()) {
	cout << "Syntax error: empty condition" << endl;
	return;
  }

  if (remainder.length() < 4 || remainder.substr(0, 4) != "then") {
	cout << "Syntax error: missing 'then' after condition" << endl;
	return;
  }

  vector<string> loopCommands;
  string loopInput;

  cout << "Enter commands (type 'end' to finish while loop):" << endl;
  while (true) {
	cout << "... ";
	if (!getline(cin, loopInput))
	  break;
	loopInput = trim(loopInput);
	if (loopInput == "end")
	  break;
	if (!loopInput.empty())
	  loopCommands.push_back(loopInput);
  }

  while (evaluateCondition(condition)) {
	for (const string &cmd : loopCommands) {
	  executeCommand(cmd);
	}
  }
}