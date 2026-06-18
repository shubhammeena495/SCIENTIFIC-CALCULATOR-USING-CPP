#include <iostream>
#include <stack>
#include <cmath>
#include <cctype>
#include <map>

using namespace std;

// Function map
map<string, double(*)(double)> functions = {
    {"sin", sin},
    {"cos", cos},
    {"log", log},
    {"sqrt", sqrt}
};

// Precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Apply operator
double applyOp(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw runtime_error("Divide by zero");
            return a / b;
        case '^': return pow(a, b);
    }
    return 0;
}

// Apply function
double applyFunction(string func, double value) {
    if (functions.find(func) != functions.end()) {
        return functions[func](value);
    }
    throw runtime_error("Unknown function: " + func);
}

// Evaluate expression
double evaluate(string expr) {
    stack<double> values;
    stack<char> ops;
    stack<string> funcStack;

    for (int i = 0; i < expr.length(); i++) {

        if (expr[i] == ' ') continue;

        // Number
        if (isdigit(expr[i]) || expr[i] == '.') {
            string val = "";
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                val += expr[i++];
            }
            i--;
            values.push(stod(val));
        }

        // Function (sin, cos, etc.)
        else if (isalpha(expr[i])) {
            string func = "";
            while (i < expr.length() && isalpha(expr[i])) {
                func += expr[i++];
            }
            i--;
            funcStack.push(func);
        }

        // Opening bracket
        else if (expr[i] == '(') {
            ops.push('(');
        }

        // Closing bracket
        else if (expr[i] == ')') {

            while (!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }

            ops.pop(); // remove '('

            // Apply function if exists
            if (!funcStack.empty()) {
                string func = funcStack.top();
                funcStack.pop();

                double val = values.top(); values.pop();
                values.push(applyFunction(func, val));
            }
        }

        // Operator
        else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(expr[i]);
        }
    }

    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    return values.top();
}

int main() {
    string expr;

    cout << "Enter expression: ";
    getline(cin, expr);

    try {
        double result = evaluate(expr);
        cout << "Result = " << result << endl;
    } catch (exception &e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}