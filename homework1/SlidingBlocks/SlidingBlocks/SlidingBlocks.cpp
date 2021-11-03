#include <iostream>
#include <list>
#include <cmath>
#include <string>
#include <stack>
#include <vector>
#include <chrono>

short int SIZE;

void swap(short int& a, short int& b)
{
    short int temp;
    temp = a;
    a = b;
    b = temp;
}

struct Node 
{
    short int** matrix;
    std::list<Node> children;
    short int emptyX;
    short int emptyY;
    std::string lastMovement;


    Node(short int** newMatrix)
    {
        matrix = new short int* [SIZE];
        for (short int i = 0; i < SIZE; i++)
            matrix[i] = new short int [SIZE];

        for (short int i = 0; i < SIZE; i++)
        {
            for (short int j = 0; j < SIZE; j++)
            {
                matrix[i][j] = newMatrix[i][j];
                if (matrix[i][j] == 0)
                {
                    emptyX = i;
                    emptyY = j;
                }
            }
        }

        lastMovement = "none";
    }

    Node(short int** newMatrix, std::string movement)
    {
        matrix = new short int* [SIZE];
        for (short int i = 0; i < SIZE; i++)
            matrix[i] = new short int[SIZE];

        for (short int i = 0; i < SIZE; i++)
        {
            for (short int j = 0; j < SIZE; j++)
            {
                matrix[i][j] = newMatrix[i][j];
                if (matrix[i][j] == 0)
                {
                    emptyX = i;
                    emptyY = j;
                }
            }
        }

        lastMovement = movement;
    }

    short int** left()
    {
        short int** childMatrix = new short int* [SIZE];
        for (short int i = 0; i < SIZE; i++)
            childMatrix[i] = new short int[SIZE];

        for (short int i = 0; i < SIZE; i++)
        {
            for (short int j = 0; j < SIZE; j++)
            {
                childMatrix[i][j] = matrix[i][j];
            }
        }

        if (emptyY < SIZE - 1)
        {
            childMatrix[emptyX][emptyY] = childMatrix[emptyX][emptyY + 1];
            childMatrix[emptyX][emptyY + 1] = 0;
            //swap(matrix[emptyX][emptyY], matrix[emptyX][emptyY + 1]);
        }

        return childMatrix;
    }

    short int** right()
    {
        short int** childMatrix = new short int* [SIZE];
        for (short int i = 0; i < SIZE; i++)
            childMatrix[i] = new short int[SIZE];

        for (short int i = 0; i < SIZE; i++)
        {
            for (short int j = 0; j < SIZE; j++)
            {
                childMatrix[i][j] = matrix[i][j];
            }
        }

        if (emptyY > 0)
        {
            childMatrix[emptyX][emptyY] = childMatrix[emptyX][emptyY - 1];
            childMatrix[emptyX][emptyY - 1] = 0;
            //swap(matrix[emptyX][emptyY], matrix[emptyX][emptyY - 1]);
        }

        return childMatrix;
    }

    short int** up()
    {
        short int** childMatrix = new short int* [SIZE];
        for (short int i = 0; i < SIZE; i++)
            childMatrix[i] = new short int[SIZE];

        for (short int i = 0; i < SIZE; i++)
        {
            for (short int j = 0; j < SIZE; j++)
            {
                childMatrix[i][j] = matrix[i][j];
            }
        }

        if (emptyX < SIZE - 1)
        {
            childMatrix[emptyX][emptyY] = childMatrix[emptyX + 1][emptyY];
            childMatrix[emptyX + 1][emptyY] = 0;
            //swap(matrix[emptyX][emptyY], matrix[emptyX + 1][emptyY]);
        }

        return childMatrix;
    }

    short int** down()
    {
        short int** childMatrix = new short int* [SIZE];
        for (short int i = 0; i < SIZE; i++)
            childMatrix[i] = new short int[SIZE];

        for (short int i = 0; i < SIZE; i++)
        {
            for (short int j = 0; j < SIZE; j++)
            {
                childMatrix[i][j] = matrix[i][j];
            }
        }

        if (emptyX > 0)
        {
            childMatrix[emptyX][emptyY] = childMatrix[emptyX - 1][emptyY];
            childMatrix[emptyX - 1][emptyY] = 0;
            //swap(matrix[emptyX][emptyY], matrix[emptyX + 1][emptyY]);
        }

        return childMatrix;
    }

    std::vector<Node> getChildren()
    {
        std::vector<Node> children;

        if (lastMovement != "right")
        {
            if (emptyY < SIZE - 1)
            {
                Node left(left(), "left");
                children.push_back(left);
            }
        }
        if (lastMovement != "left")
        {
            if (emptyY > 0)
            {
                Node right(right(), "right");
                children.push_back(right);
            }
        }
        if (lastMovement != "down")
        {
            if (emptyX < SIZE - 1)
            {
                Node up(up(), "up");
                children.push_back(up);
            }
        }
        if (lastMovement != "up")
        {
            if (emptyX > 0)
            {
                Node down(down(), "down");
                children.push_back(down);
            }
        }


        return children;
    }

    void print()
    {
        for (short int i = 0; i < SIZE; i++)
        {
            for (short int j = 0; j < SIZE; j++)
            {
                std::cout << matrix[i][j] << ' ';
            }
            std::cout << '\n';
        }

        std::cout << '\n';
    }
};

short int elementHeuristic(short int elem, short int x, short int y)
{
    int elemX, elemY;//, sum;
    elemX = elem / SIZE;
    elemY = (elem % SIZE) - 1;
    if (elemY == -1)
    {
        elemY = SIZE - 1;
        elemX--;
    }
    //sum = abs((elemX - x)) + abs((elemY - y));

    //std::cout << "elemX = " << elemX << '\n';
    //std::cout << "elemY = " << elemY << '\n';
    //std::cout << "x = " << x << '\n';
    //std::cout << "y = " << y << '\n';

    return abs((elemX - x)) + abs((elemY - y));
}

short int manhattan_heuristic(short int** matrix, short int goalIndex)
{
    short int heuristicSum = 0;
    bool passedZero = false;

    for (short int i = 0; i < SIZE; i++)
    {
        for (short int j = 0; j < SIZE; j++)
        {
            if (matrix[i][j] == 0)
            {
                continue;
            }

            if (matrix[i][j] < goalIndex)
            {
                heuristicSum += elementHeuristic(matrix[i][j], i, j);
            }
            else
            {
                heuristicSum += elementHeuristic(matrix[i][j] + 1, i, j);
            }
        }
    }

    return heuristicSum;
}

short int** generateGoalMatrix(short int goalIndex)
{
    short int** goalMatrix;
    goalMatrix = new short int* [SIZE];
    for (short int i = 0; i < SIZE; i++)
        goalMatrix[i] = new short int[SIZE];


    short int index = 1;
    bool doOnce = true;
    for (short int i = 0; i < SIZE; i++)
    {
        for (short int j = 0; j < SIZE; j++)
        {
            if (doOnce && goalIndex == index)
            {
                goalMatrix[i][j] = 0;
                doOnce = false;
            }
            else
            {
                goalMatrix[i][j] = index++;
            }
        }
    }

    return goalMatrix;
}

bool isGoal(short int** matrix, short int** goalMatrix)
{
    for (short int i = 0; i < SIZE; i++)
    {
        for (short int j = 0; j < SIZE; j++)
        {
            if (matrix[i][j] != goalMatrix[i][j])
                return false;
        }
    }

    return true;
}

short int min(std::stack<short int>& boundries)
{
    short int min = boundries.top();
    boundries.pop();

    while (!boundries.empty())
    {
        if (min > boundries.top())
        {
            min = boundries.top();
        }

        boundries.pop();
    }

    return min;
}

short int max(std::stack<short int>& boundries)
{
    short int max = boundries.top();
    boundries.pop();

    while (!boundries.empty())
    {
        if (max < boundries.top())
        {
            max = boundries.top();
        }

        boundries.pop();
    }

    return max;
}

//algorithm


bool algorithm(Node crr, short int** goalMatrix, short int goalIndex, std::stack<std::string>& path, short int boundry , std::stack<short int>& boundries, short int rootHeuristic)
{
    if (isGoal(crr.matrix, goalMatrix))
    {
        return true; //found
    }

    std::vector<Node> children = crr.getChildren();

    short int heuristic; 
    for (std::vector<Node>::iterator it = children.begin(); it != children.end(); it++)
    {
        heuristic = manhattan_heuristic(it->matrix, goalIndex);

        if (heuristic == 0)
        {
            path.push(it->lastMovement);
            return true;
        }

        if ((rootHeuristic + heuristic) > boundry)
        {
            boundries.push(rootHeuristic + heuristic);
        }
        else
        {
            path.push(it->lastMovement);
            if (algorithm(*it, goalMatrix, goalIndex, path, boundry, boundries, rootHeuristic + heuristic))
                return true;
        }
    }

    return false;
}

void ida_star(Node root, short int** goalMatrix, short int goalIndex)
{
    auto frame_start = std::chrono::high_resolution_clock::now();

    short int rootHeuristic = manhattan_heuristic(root.matrix, goalIndex);
    short int boundry = rootHeuristic + 1;
    std::stack<std::string> path;
    std::stack<short int> boundries;

    while (true)
    {
        if (algorithm(root, goalMatrix, goalIndex, path, boundry, boundries, rootHeuristic))
        {
            break;
        }
        boundry = min(boundries);

        while (!path.empty())
        {
            path.pop();
        }
    }


    auto frame_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = frame_end - frame_start;

    std::cout << duration.count() << '\n';


    std::string result;
    std::cout << path.size() << '\n';
    while (!path.empty())
    {
        result = (path.top() + "\n") + result;
        path.pop();
    }
    std::cout << result;
}



int main()
{
    short int count, goalIndex;
    short int** matrix;

    //input data
    std::cin >> count;
    SIZE = sqrt(count + 1);

    std::cin >> goalIndex;
    if (goalIndex == -1)
        goalIndex = count + 1;

    matrix = new short int* [SIZE];
    for (short int i = 0; i < SIZE; i++)
        matrix[i] = new short int[SIZE];

    for (short int i = 0; i < SIZE; i++)
    {
        for (short int j = 0; j < SIZE; j++)
        {
            std::cin >> matrix[i][j];
        }
    }
    
    short int** goalMatrix = generateGoalMatrix(goalIndex);

    //std::cout <<std::boolalpha<< isGoal(matrix, goalMatrix);

    //generate first node
    Node node(matrix);

    
    ida_star(node, goalMatrix, goalIndex);
    /*
    std::cout << "\n=====================\n";
    
    std::cout << '\n';
    std::cout << manhattan_heuristic(node.matrix, goalIndex) << '\n';
    node.print();
    std::cout << '\n';

    std::vector<Node> nodes = node.getChildren();

    std::cout << "---------------\n";

    for (std::vector<Node>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        std::cout << manhattan_heuristic(it->matrix, goalIndex) << '\n';
        it->print();
        std::cout << '\n';
    }

    std::cout << "---------------\n";

    std::vector<Node> nodes2 = nodes.front().getChildren();

    for (std::vector<Node>::iterator it = nodes2.begin(); it != nodes2.end(); it++)
    {
        std::cout << manhattan_heuristic(it->matrix, goalIndex) << '\n';
        it->print();
        std::cout << '\n';
    }

    Node node2(node.left());

    std::cout << '\n';



    for (short int i = 0; i < SIZE; i++)
    {
        for (short int j = 0; j < SIZE; j++)
        {
            std::cout << node2.matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }

    std::cout << '\n';

    */
    //generate goal matrix
    /*short int** goalMatrix;
    goalMatrix = new short int* [SIZE];
    for (short int i = 0; i < SIZE; i++)
        goalMatrix[i] = new short int[SIZE];


    short int index = 1;
    bool doOnce = true;
    for (short int i = 0; i < SIZE; i++)
    {
        for (short int j = 0; j < SIZE; j++)
        {
            if (doOnce && goalIndex == index)
            {
                goalMatrix[i][j] = 0;
                doOnce = false;
            }
            else
            {
                goalMatrix[i][j] = index++;
            }
        }
    }

    for (short int i = 0; i < SIZE; i++)
    {
        for (short int j = 0; j < SIZE; j++)
        {
            std::cout << goalMatrix[i][j] << ' ';
        }
        std::cout << '\n';
    }*/

    //calculate manhattan heuristic

    //short int heuristicSum = 0;
    //bool passedZero = false;

    //for (short int i = 0; i < SIZE; i++)
    //{
    //    for (short int j = 0; j < SIZE; j++)
    //    {
    //        if (matrix[i][j] == 0)
    //        {
    //            continue;
    //        }

    //        if (matrix[i][j] < goalIndex)
    //        {
    //            heuristicSum += elementHeuristic(matrix[i][j], i, j);
    //            //std::cout << matrix[i][j] << '|' << elementHeuristic(matrix[i][j], i, j) << '\n';
    //        }
    //        else
    //        {
    //            heuristicSum += elementHeuristic(matrix[i][j] + 1, i, j);
    //            //std::cout << matrix[i][j] << '/' << elementHeuristic(matrix[i][j], i, j) << '\n';
    //        }
    //    }
    //}

    //std::cout << '\n' << heuristicSum;
    
}
