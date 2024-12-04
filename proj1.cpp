// Created by: Leonardo Neves :)
// Created on: 2024-12-04

#include <cstdio>
#include <iostream>
#include <vector>
#include <algorithm>

#define VEC_CONST 5

using namespace std;

int** TABLE_PTR;

int N_MAX;
int M_MAX;
vector<int> MAIN_ARRAY;
vector<vector<vector<int>>> DATABASE;

void printDatabase () {
    for (int a = 0; a < M_MAX; a++) {
        for (int b = 0; b < M_MAX; b++) {
            putchar('{');
            for (int i : DATABASE[a][b]){
                printf("%d,",i);
            }
            putchar('}');
            putchar(' ');
        }
        putchar('\n');
    }
}

// formato do inner vector apenas na 1a diagonal {0 (indicates we are in terminal case), value}
void initializeDataBase () {
    for (int i = 0; i < M_MAX; i++)
    {
        DATABASE[i][i].push_back(3);
        DATABASE[i][i].push_back(-1);
        DATABASE[i][i].push_back(MAIN_ARRAY[i]);
    }
}

// formato dos inner inner vectors {size,IsMax,[value,leftcol,leftposition,rightrow,rightposition],[...],[...]}

// function to calculate the vector of a cell based on 2 other vectors and their positions on the table
vector<int> merge(int row, int col) {

    vector<int> newVec;
    newVec.push_back(2); // current size
    newVec.push_back(0); // isMax
    
    int leftSide = col - 1;
    int rightSide = col; 

   
    while (rightSide > row) {
        vector<int> right =  DATABASE[rightSide][col];
        vector<int> left = DATABASE[row][leftSide];


        for (int countLeft = 2; countLeft < left[0]; countLeft += VEC_CONST) {

            for (int countRight = 2; countRight < right[0]; countRight += VEC_CONST) {

                int isIn = 0;
                for (int countVec = 2; countVec < newVec[0]; countVec += VEC_CONST) {
                    if (TABLE_PTR[left[countLeft] - 1][right[countRight] - 1] == newVec[countVec]) {
                        isIn = 1;
                        break;
                    }
                }

                if (isIn == 0)
                {
                    newVec[0] += VEC_CONST;
                    newVec.push_back(TABLE_PTR[left[countLeft] - 1][right[countRight] - 1]);
                    newVec.push_back(leftSide);
                    newVec.push_back(countLeft);
                    newVec.push_back(rightSide);
                    newVec.push_back(countRight);
                }

                if (((newVec[0] - 2) / VEC_CONST) > N_MAX)
                {
                    printf("OVERFLOW\n"); // if appears on the output, something went wrong
                }

                if (((newVec[0] - 2) / VEC_CONST) == N_MAX) // if the vector is complete
                {
                    newVec[1] = 1;
                    break;
                }
            }

            if (newVec[1] == 1)
            {
                break;
            }
        }

        if (newVec[1] == 1) 
        {
            break;
        }
        

        rightSide--;
        leftSide--;
    }


    return newVec;
}


void printVector(int row, int col, int pos) {

   
    if (DATABASE[row][col][1] == -1)
    {
        printf("%d", DATABASE[row][col][2]);
    }
    
    else {

        putchar('(');
        printVector(row, DATABASE[row][col][pos + 1], DATABASE[row][col][pos + 2]);
        putchar(' ');
        printVector(DATABASE[row][col][pos + 3], col, DATABASE[row][col][pos + 4]);
        putchar(')');
    }
}


int main() {

    // INPUT VARIABLES
    int n, m, result;
    if (scanf("%d%d", &n, &m) != 2) {
        fprintf(stderr, "Error reading n and m\n");
        return 1;
    }
    int** table = new int*[n]; 
    for (int i = 0; i < n; i++) {
        table[i] = new int[n];
        for (int j = 0; j < n; j++) {
            if (scanf("%d",&table[i][j]) != 1) {
                fprintf(stderr, "Error reading n and m\n");
                return 1;
            }
        }
    }
    vector<int> array(m);
    for (int i = 0; i < m; i++) { // loop for input vector
        if (scanf("%d",&array[i]) != 1) {
            fprintf(stderr, "Error reading n and m\n");
            return 1;
        }
    }
    if (scanf("%d",&result) != 1) {
        fprintf(stderr, "Error reading n and m\n");
        return 1;
    }
    

    // END OF INPUT VARIABLES

    // formato dos inner vectors {size,(value,leftrow,leftcol,leftposition,rightrow,rightcol,rightposition),()}



    TABLE_PTR = table;
    vector<vector<vector<int>>> data(m, vector<vector<int>>(m));
    DATABASE = data;
    N_MAX = n;
    M_MAX = m;
    MAIN_ARRAY = array;

    int row = 0;
    initializeDataBase();

    
    for(int a = 1; a < m; a++){
        row = 0;

        for (int col = a; col < m; col++) {

          
            DATABASE[row][col] = merge(row,col);
            row++;


        }
    }

    
    int valid = 0;
    
    for (int o = 2; o < DATABASE[0][M_MAX - 1][0]; o += VEC_CONST)
    {
        if (DATABASE[0][M_MAX - 1][o] == result)
        {
            valid = 1;
            putchar('1');
            putchar('\n');
            printVector(0, M_MAX - 1, o);
            putchar('\n');
            break;
        }
    }

    if (valid == 0) {
        putchar('0');
        putchar('\n');
    }
    

    for (int i = 0; i < n; ++i) {
        delete[] table[i];  // Free each row
    }
    delete[] table; 

    return 0;
}

