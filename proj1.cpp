#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

/// Estruturas globais
vector<vector<int>> graph;   // Tabela de operações
vector<int> expression;      // Expressão inicial
int result;                  // Resultado desejado

// Estrutura para armazenar resultados de subintervalos
struct Result {
    unordered_map<int, string> expressions; // Mapeia valores para suas expressões
};

// Tabela dinâmica
vector<vector<Result>> dp;

void solve() {
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

    // Leitura do número de operações e da expressão
    cin >> n >> m;

    graph.resize(n, vector<int>(n));
    expression.resize(m);

    // Preenche a tabela de operações
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> graph[i][j];
        }
    }

    // Leitura da expressão
    for (int i = 0; i < m; ++i) {
        cin >> expression[i];
    }

    // Leitura do resultado desejado
    cin >> result;
}

int main() {
    // Leitura dos dados
    readInput();

    // Solução
    solve();

    // Verifica o resultado no intervalo completo
    if (dp[0][expression.size() - 1].expressions.count(result)) {
        cout << "1\n" << dp[0][expression.size() - 1].expressions[result] << "\n";
    } else {
        cout << "0\n";
    }

    return 0;
}
