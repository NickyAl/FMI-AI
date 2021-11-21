#include <iostream>
#include <vector>
#include <stack>
#include <random>
#include <time.h>
#include <chrono>

int SIZE;

struct Couple
{
    int val;
    int index;
};

int collisions(int x, int y, int* board, int* lines, int* mainDiag, int* scndDiag)
{
    return lines[y] + mainDiag[(x - y) + (SIZE - 1)] + scndDiag[x + y] - 3;
}

int indexOfMaxCollisions(int* board, int* lines, int* mainDiag, int* scndDiag)
{
    int max = 0;
    std::stack<Couple> couples;
    std::vector<int> maxIndexes;

    int val;
    for (int i = 0; i < SIZE; i++)
    {
        val = collisions(i, board[i], board, lines, mainDiag, scndDiag);
        Couple cpl{val, i};

        if (val >= max)
        {
            couples.push(cpl);
            max = val;
        }
    }

    if (max == 0)
        return -1;

    while (!couples.empty())
    {
        if (couples.top().val == max)
            maxIndexes.push_back(couples.top().index);

        couples.pop();
    }
    
    return maxIndexes[rand() % maxIndexes.size()];
}

int indexOfMinCollisions(int x, int* board, int* lines, int* mainDiag, int* scndDiag)
{
    int min = INT_MAX;
    std::stack<Couple> couples;
    std::vector<int> minIndexes;

    int val;
    for (int i = 0; i < SIZE; i++)
    {
        val = collisions(x, i, board, lines, mainDiag, scndDiag);
        Couple cpl{ val, i };

        if (val <= min)
        {
            couples.push(cpl);
            min = val;
        }
    }

    while (!couples.empty())
    {
        if (couples.top().val == min)
            minIndexes.push_back(couples.top().index);

        couples.pop();
    }

    return minIndexes[rand() % minIndexes.size()];
}

void init(int* board, int* lines, int* mainDiag, int* scndDiag)
{
    int bottom = SIZE / 2,
        diagSize = SIZE * 2 - 1,
        sumMD = 0, sumSD = 0, sumLines = 0;

    for (int i = 0; i < SIZE; i++)
    {
        if (i % 2 == 0)
        {
            board[i] = bottom + (i / 2);
        }
        else
        {
            board[i] = i/2;
        }
    }

    for (int i = 0; i < diagSize; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (j - board[j] == i - (SIZE - 1))
            {
                sumMD++;
            }

            if (j + board[j] == i)
            {
                sumSD++;
            }

            if (i < SIZE)
            {
                if (board[j] == i)
                    sumLines++;
            }
        }

        mainDiag[i] = sumMD;
        sumMD = 0;

        scndDiag[i] = sumSD;
        sumSD = 0;

        if (i < SIZE)
        {
            lines[i] = sumLines;
            sumLines = 0;
        }
    }
}


void solve(int* board, int* lines, int* mainDiag, int* scndDiag, std::chrono::steady_clock::time_point frame_start)
{
    if (SIZE == 1)
    {
        std::cout << "*\n";
        return;
    }

    if (SIZE == 2 || SIZE == 3)
    {
        std::cout << "Impossible\n";
        return;
    }

    init(board, lines, mainDiag, scndDiag);

    int iter = 0, col, row;
    while (iter++ <= 3 * SIZE)
    {
        col = indexOfMaxCollisions(board, lines, mainDiag, scndDiag);

        if (col == -1)
        {
            auto frame_end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> duration = frame_end - frame_start;
            std::cout << duration.count() << '\n';

            if (SIZE <= 40)
            {
                for (int i = 0; i < SIZE; i++)
                {
                    for (int j = 0; j < SIZE; j++)
                    {
                        if (board[j] == i)
                            std::cout << "* ";
                        else
                            std::cout << "_ ";
                    }
                    std::cout << '\n';
                }
            }
            else
            {
                std::cout << "FOUND\n";
            }
            return;
        }

        row = indexOfMinCollisions(col, board, lines, mainDiag, scndDiag);

        lines[board[col]]--;
        mainDiag[(col - board[col]) + (SIZE - 1)]--;
        scndDiag[col + board[col]]--;

        board[col] = row;

        lines[board[col]]++;
        mainDiag[(col - board[col]) + (SIZE - 1)]++;
        scndDiag[col + board[col]]++;  
    }

    solve(board, lines, mainDiag, scndDiag, frame_start);
}

int main()
{
    srand((unsigned)time(NULL));
    std::cin >> SIZE;

    int* board = new int[SIZE];
    int* lines = new int[SIZE];
    int* mainDiag = new int[SIZE * 2 - 1];
    int* scndDiag = new int[SIZE * 2 - 1];
    auto frame_start = std::chrono::high_resolution_clock::now();

    solve(board, lines, mainDiag, scndDiag ,frame_start);

    delete[] board;
    delete[] lines;
    delete[] mainDiag;
    delete[] scndDiag;

    return 0;
}