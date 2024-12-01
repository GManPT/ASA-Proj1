#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <unordered_map>

using namespace std;

/// Global variables
/// graph: table of operations
/// expression: expression to be evaluated
/// result: desired result
vector<vector<int>> graph;
vector<int> expression;
int result;

// Structure to store the result of a subinterval
struct Result {
    unordered_set<int> values;
    unordered_map<int, string> parenthesis;
};

// Table to store the results of subintervals
vector<vector<Result>> dp;


void lvsolve() {
    int m = expression.size();

    for (int len = 1; len <= m; ++len) { 
        cout << "\n\n-----------TAMANHO DO INTERVALO: " << len << "-----------" << endl;
        for (int i = 0; i <= m - len; ++i) { 
            int j = i + len - 1;

            if (i == j) {
                // Base case: single element
                dp[i][j].values.insert(expression[i]);
                dp[i][j].parenthesis[expression[i]] = to_string(expression[i]);
            } else {
                Result currentResult;

                // Divide the interval in two subintervals
                for (int k = i; k < j; ++k) {
                    Result &left = dp[i][k];
                    Result &right = dp[k + 1][j];

                    // Combine subintervals
                    for (int lval : left.values) {
                        for (int rval : right.values) {
                            if (lval >= 1 && lval <= graph.size() && rval >= 1 && rval <= graph.size()) {
                                int new_val = graph[lval - 1][rval - 1];
                                currentResult.values.insert(new_val);

                                // Creates the desired expression
                                string l_expr = left.parenthesis[lval];
                                string r_expr = right.parenthesis[rval];
                                currentResult.parenthesis[new_val] = "(" + l_expr + " " + r_expr + ")";
                            }
                        }
                    }
                }

                dp[i][j] = currentResult;
                
                // DEBUGGING
                cout << "\nIntervalo [" << i << ", " << j << "]:\n";

                for (int val : currentResult.values) {
                    cout << val << " e expressão " << currentResult.parenthesis[val] << endl;
                }

            }
        }
    }
}

void readInput() {
    int n, m;

    // Read the number of operations and the number of elements in the expression
    cin >> n >> m;

    // Resize the data structures
    graph.resize(n, vector<int>(n));
    expression.resize(m);
    dp.resize(m, vector<Result>(m));

    // Fill the table of operations
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> graph[i][j];
        }
    }

    // Read the expression
    for (int i = 0; i < m; ++i) {
        cin >> expression[i];
    }

    // Read the desired result
    cin >> result;
}

int main() {
    // Read input data
    readInput();

    // Solve the problem
    lvsolve();

    // Print the result
    cout << "\n\n\n";

    if (dp[0][expression.size() - 1].values.count(result)) {
        cout << "1\n" << dp[0][expression.size() - 1].parenthesis[result] << "\n";
    } else {
        cout << "0\n";
    }

    return 0;
}
