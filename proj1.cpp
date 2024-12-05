#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

#define VEC_CONST 5

using namespace std;

/// INPUT VARIABLES
/// n: Size of given table
/// m: Size of sequence
/// result: Desired value
/// table: Given table with n size
/// sequence: Given sequence with m size
int n, m, result;
vector<vector<int>> table;
vector<int> sequence;


/// DP VARIABLES
vector<vector<vector<int>>> dp;


/// Initialize dp table 1st diagonal.
/// Note that {0, value} is the terminal case later used.
/// @return none
void initializeDataBase () {
    for (int i = 0; i < m; i++) {
        dp[i][i].push_back(3);
        dp[i][i].push_back(-1);
        dp[i][i].push_back(sequence[i]);
    }
}


/// Read input values from stdin
/// @return none
void readInput() {
    // Fast I/O (Leaks memory)
    ios::sync_with_stdio(0);
    cin.tie(0);

    // Read m and n values
    cin >> n >> m;

    // Resize table, sequence and dp
    table.resize(n, vector<int>(n));
    sequence.resize(m);
    dp.resize(m, vector<vector<int>>(m));


    // Read table values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> table[i][j];
        }
    }

    // Read sequence values
    for (int i = 0; i < m; i++) {
        cin >> sequence[i];
    }

    // Read desired value
    cin >> result;
}


/// Calculate the vector of a cell based on 2 other vectors
/// and their positions on the table
/// @param row: Row of the cell
/// @param col: Column of the cell
/// @return vector<int>: Calculated vector
vector<int> merge (int row, int col) {
    // 2 is current size and the 0 max value
    vector<int> newVec = {2, 0};

    int leftSide = col - 1;
    int rightSide = col;
    int isIn;

    while (rightSide > row) {
        vector<int> left = dp[row][leftSide];
        vector<int> right = dp[rightSide][col];

        for (int countLeft = 2; countLeft < left[0]; countLeft += VEC_CONST) {
            for (int countRight = 2; countRight < right[0]; countRight += VEC_CONST) {
                isIn = 0;

                // Check if the value is already in the vector
                for (int countVec = 2; countVec < newVec[0]; countVec += VEC_CONST) {
                    if (table[left[countLeft] - 1][right[countRight] - 1] == newVec[countVec]) {
                        isIn = 1;
                        break;
                    }
                }

                // If it is not in the vector, add it
                if (!isIn) {
                    newVec[0] += VEC_CONST;
                    newVec.push_back(table[left[countLeft] - 1][right[countRight] - 1]);
                    newVec.push_back(leftSide);
                    newVec.push_back(countLeft);
                    newVec.push_back(rightSide);
                    newVec.push_back(countRight);
                }

                // If the vector is full, break
                if (((newVec[0] - 2) / VEC_CONST) == n) {
                    newVec[1] = 1;
                    break;
                }
                
            }

            if (newVec[1]) break;
        }

        if (newVec[1]) break;
        rightSide--;
        leftSide--;
    }
    return newVec;
}


/// Solve the problem
/// @return none
void solve() {
    int row;

    for (int i = 1; i < m; i++) {
        row = 0;

        for (int col = i; col < m; col++) {
            dp[row][col] = merge(row, col);
            row++;
        }
    }
}


// Print the vector (final expression)
/// @param row: Row of the cell
/// @param col: Column of the cell
/// @param pos: Position of the vector
/// @return none
void printVector(int row, int col, int pos) {
    // Base case: If the vector is terminal
    if (dp[row][col][1] == -1) cout << dp[row][col][2];
    else {
        cout << "(";
        printVector(row, dp[row][col][pos + 1], dp[row][col][pos + 2]);
        cout << " ";
        printVector(dp[row][col][pos + 3], col, dp[row][col][pos + 4]);
        cout << ")";
    }
}


/// Print the solution
/// @return none
void printSolution() {
    for (int i = 2; i < dp[0][m - 1][0]; i += VEC_CONST) {
        // If the result is found, print it
        if (dp[0][m - 1][i] == result) {
            cout << "1" << endl;
            printVector(0, m - 1, i);
            return;
        }
    }

    // If the result is not found, print 0
    cout << "0" << endl;
}


int main() {
    readInput();
    initializeDataBase();
    solve();
    printSolution();
}
