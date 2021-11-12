#include <iostream>
#include <cmath>
#include <string>
#include <stack>
#include <vector>
#include <chrono>

using s_int = short int;

s_int SIZE;

class Node
{
private:
    s_int** matrix;
    s_int emptyX, emptyY;

    std::string lastMovement;

    //helper functions to generate the right matrix for the second constructor
    s_int** left();
    s_int** right();
    s_int** up();
    s_int** down();

    static int elementHeuristic(s_int elem, s_int x, s_int y);

public:
    Node(s_int** newMatrix);
    Node(s_int** newMatrix, std::string movement); //the second constructor
    ~Node();

    std::string getLastMovement() const { return lastMovement; }

    std::vector<Node*> getChildren();

    int manhattan_heuristic(s_int goalIndex);

    bool isGoal(s_int** goalMatrix);
};

//Helper functions

s_int** generateGoalMatrix(s_int goalIndex)
{
    s_int** goalMatrix;
    goalMatrix = new s_int * [SIZE];
    for (s_int i = 0; i < SIZE; i++)
        goalMatrix[i] = new s_int[SIZE];


    s_int index = 1;
    bool doOnce = true;
    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            if (doOnce && goalIndex == index) //to place the empty space
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

int min(std::stack<int>& boundries)
{
    int min = boundries.top();
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

//Algorithm

bool search(Node* crr, s_int** goalMatrix, s_int goalIndex, std::stack<std::string>& path, int boundry, std::stack<int>& boundries, int rootHeuristic)
{
    if (crr->isGoal(goalMatrix))
    {
        return true; //the root is the goal
    }

    std::vector<Node*> children = crr->getChildren();
    int heuristic;
    Node* child;
    for (std::vector<Node*>::iterator it = children.begin(); it != children.end(); it++)
    {
        child = *it;
        heuristic = child->manhattan_heuristic(goalIndex);

        if (heuristic == 0)
        {
            path.push(child->getLastMovement());
            delete child;
            return true;
        }

        if ((rootHeuristic + heuristic) > boundry)
        {
            boundries.push(rootHeuristic + heuristic);
        }
        else
        {

            if (search(child, goalMatrix, goalIndex, path, boundry, boundries, rootHeuristic + heuristic))
            {
                path.push(child->getLastMovement());
                delete child;
                return true;
            }
        }

        delete child;
    }

    return false;
}

void ida_star(Node* root, short int** goalMatrix, short int goalIndex)
{
    auto frame_start = std::chrono::high_resolution_clock::now();

    short int rootHeuristic = root->manhattan_heuristic(goalIndex);
    short int boundry = rootHeuristic + 1;
    std::stack<std::string> path;
    std::stack<int> boundries;

    while (true)
    {
        if (search(root, goalMatrix, goalIndex, path, boundry, boundries, rootHeuristic))
        {
            break;
        }
        boundry = min(boundries);

        while (!boundries.empty())
        {
            boundries.pop();
        }

        while (!path.empty())
        {
            path.pop();
        }
    }


    auto frame_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = frame_end - frame_start;

    std::cout << duration.count() << '\n';

    std::cout << path.size() << '\n';

    while (!path.empty())
    {
        std::cout << path.top() << '\n';
        path.pop();
    }
}


int main()
{
    s_int count, goalIndex;
    s_int** matrix;

    //input data
    std::cin >> count;
    SIZE = (int)sqrt(count + 1);

    std::cin >> goalIndex;
    if (goalIndex == -1)
        goalIndex = count + 1;

    matrix = new short int* [SIZE];
    for (s_int i = 0; i < SIZE; i++)
        matrix[i] = new s_int[SIZE];

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            std::cin >> matrix[i][j];
        }
    }

    short int** goalMatrix = generateGoalMatrix(goalIndex);

    Node* node = new Node(matrix);

    ida_star(node, goalMatrix, goalIndex);

    delete node;

    for (s_int i = 0; i < SIZE; i++)
    {
        delete[] matrix[i];
        delete[] goalMatrix[i];
    }
    delete[] matrix;
    delete[] goalMatrix;
}

s_int** Node::left()
{
    s_int** childMatrix = new s_int * [SIZE];
    for (s_int i = 0; i < SIZE; i++)
        childMatrix[i] = new s_int[SIZE];

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            childMatrix[i][j] = matrix[i][j];
        }
    }

    childMatrix[emptyX][emptyY] = childMatrix[emptyX][emptyY + 1];
    childMatrix[emptyX][emptyY + 1] = 0;

    return childMatrix;
}

s_int** Node::right()
{
    s_int** childMatrix = new s_int * [SIZE];
    for (s_int i = 0; i < SIZE; i++)
        childMatrix[i] = new s_int[SIZE];

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            childMatrix[i][j] = matrix[i][j];
        }
    }

    childMatrix[emptyX][emptyY] = childMatrix[emptyX][emptyY - 1];
    childMatrix[emptyX][emptyY - 1] = 0;

    return childMatrix;
}

s_int** Node::up()
{
    s_int** childMatrix = new s_int * [SIZE];
    for (s_int i = 0; i < SIZE; i++)
        childMatrix[i] = new s_int[SIZE];

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            childMatrix[i][j] = matrix[i][j];
        }
    }

    childMatrix[emptyX][emptyY] = childMatrix[emptyX + 1][emptyY];
    childMatrix[emptyX + 1][emptyY] = 0;

    return childMatrix;
}

s_int** Node::down()
{
    s_int** childMatrix = new s_int * [SIZE];
    for (s_int i = 0; i < SIZE; i++)
        childMatrix[i] = new s_int[SIZE];

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            childMatrix[i][j] = matrix[i][j];
        }
    }

    childMatrix[emptyX][emptyY] = childMatrix[emptyX - 1][emptyY];
    childMatrix[emptyX - 1][emptyY] = 0;

    return childMatrix;
}

int Node::elementHeuristic(s_int elem, s_int x, s_int y)
{
    int elemX, elemY;
    elemX = (int)elem / SIZE;
    elemY = ((int)elem % SIZE) - 1;
    if (elemY == -1)
    {
        elemY = SIZE - 1;
        elemX--;
    }

    return abs((elemX - (int)x)) + abs((elemY - (int)y));
}

Node::Node(s_int** newMatrix)
{
    matrix = new s_int * [SIZE];
    for (s_int i = 0; i < SIZE; i++)
        matrix[i] = new s_int[SIZE];

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
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

Node::Node(s_int** newMatrix, std::string movement)
{
    matrix = newMatrix; //we must not create a new pointer for the node's matrix because newMatrix is used only to generate a matrix for this node and we need to use it so it can be deleted with the node itself later on

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            if (matrix[i][j] == 0)
            {
                emptyX = i;
                emptyY = j;
            }
        }
    }

    lastMovement = movement;
}

Node::~Node()
{
    for (short int i = 0; i < SIZE; i++)
    {
        delete[] matrix[i];
    }

    delete[] matrix;
}

std::vector<Node*> Node::getChildren()
{
    std::vector<Node*> children;

    if (lastMovement != "right") //generates all possible children without the one that has the same matrix as the grand parrent
    {
        if (emptyY < SIZE - 1)
        {
            children.push_back(new Node(left(), "left"));
        }
    }
    if (lastMovement != "left")
    {
        if (emptyY > 0)
        {
            children.push_back(new Node(right(), "right"));
        }
    }
    if (lastMovement != "down")
    {
        if (emptyX < SIZE - 1)
        {
            children.push_back(new Node(up(), "up"));
        }
    }
    if (lastMovement != "up")
    {
        if (emptyX > 0)
        {
            children.push_back(new Node(down(), "down"));
        }
    }

    return children;
}

int Node::manhattan_heuristic(s_int goalIndex)
{
    int heuristicSum = 0;
    bool passedZero = false;

    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
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

bool Node::isGoal(s_int** goalMatrix)
{
    for (s_int i = 0; i < SIZE; i++)
    {
        for (s_int j = 0; j < SIZE; j++)
        {
            if (matrix[i][j] != goalMatrix[i][j])
                return false;
        }
    }

    return true;
}
