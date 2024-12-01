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
    unordered_map<int, string> expressions;
};

// Table to store the results of subintervals
vector<vector<Result>> dp;


void lvsolve() {
    int m = expression.size();
    dp.resize(m, vector<Result>(m));

    // Base: Intervalos unitários
    for (int i = 0; i < m; ++i) {
        dp[i][i].expressions[expression[i]] = to_string(expression[i]);
    }

    // Construção dinâmica para intervalos maiores
    for (int len = 2; len <= m; ++len) { // Tamanhos dos intervalos
        for (int i = 0; i <= m - len; ++i) { // Início do intervalo
            int j = i + len - 1; // Fim do intervalo

            // Dividimos o intervalo em dois subintervalos
            for (int k = i; k < j; ++k) {
                Result &left = dp[i][k];
                Result &right = dp[k + 1][j];

                // Combinação dos resultados dos dois subintervalos
                for (const auto &[lval, lstr] : left.expressions) {
                    for (const auto &[rval, rstr] : right.expressions) {
                        if (lval >= 1 && lval <= graph.size() && rval >= 1 && rval <= graph.size()) {
                            int new_val = graph[lval - 1][rval - 1];
                            string new_expr = "(" + lstr + " " + rstr + ")";

                            // Se não existe ou a nova expressão é mais à esquerda, substituímos
                            if (dp[i][j].expressions.find(new_val) == dp[i][j].expressions.end() ||
                                new_expr < dp[i][j].expressions[new_val]) {
                                dp[i][j].expressions[new_val] = new_expr;
                            }
                        }
                    }
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
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);

    // Read input data
    readInput();

    // Solve the problem
    lvsolve();

    if (dp[0][expression.size() - 1].expressions.count(result)) {
        cout << "1\n" << dp[0][expression.size() - 1].expressions[result] << "\n";
    } else {
        cout << "0\n";
    }

    return 0;
}
