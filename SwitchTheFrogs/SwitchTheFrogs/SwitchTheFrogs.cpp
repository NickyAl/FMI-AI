#include <iostream>
#include <string>
#include <vector>

struct Node
{
    std::string key; //the current state of the game examples: LL_RR, L_RLR
    std::vector<Node*> children;
};

Node* pushBack(std::string key, Node* perant)
{
    Node* temp = new Node{ key };
    perant->children.push_back(temp);
    return temp;
}

void printTree(Node* node)
{
    if (node == NULL)
        return;

    std::cout << node->key;
    if (!node->children.empty())
    {
        std::cout << '(';
        for (std::vector<Node*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
        {
            printTree(*it);
        }
        std::cout << ')';
    }
    std::cout << ' ';
}

std::string swap(std::string key, size_t first, size_t second)
{
    char temp = key[first];
    key[first] = key[second];
    key[second] = temp;
    return key;
}

void generateTree(Node* node, size_t size)
{
    if (node == nullptr || size < 3)
        return;

    for (size_t i = 0; i < size; i++)
    {
        if (node->key[i] == 'L')
        {
            if (i < size - 1 && node->key[i + 1] == '_')
            {
                generateTree(pushBack(swap(node->key, i, i + 1), node), size);
            }
            if (i < size - 2 && node->key[i + 2] == '_')
            {
                generateTree(pushBack(swap(node->key, i, i + 2), node), size);
            }
        }
        else if (node->key[i] == 'R')
        {
            if (i > 0 && node->key[i - 1] == '_')
            {
                generateTree(pushBack(swap(node->key, i, i - 1), node), size);
            }
            if (i > 1 && node->key[i - 2] == '_')
            {
                generateTree(pushBack(swap(node->key, i, i - 2), node), size);
            }
        }
    }
}

bool dfs(Node* node, std::vector<std::string>& result, size_t size, std::string& goal)
{
    if (node == nullptr || size < 3)
        return false;

    if (node->key == goal)
    {
        result.push_back(node->key);
        return true;
    }

    if (!node->children.empty())
    {
        for (std::vector<Node*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
        {
            if (dfs(*it, result, size, goal))
            {
                result.push_back(node->key);
                return true;
            }
        }
    }

    return false;
}


std::vector<std::string> findGoal(Node* node, size_t size, std::string& goal)
{
    std::vector<std::string> result;

    if (node == nullptr)
        return result;

    dfs(node, result, size, goal);

    return result;
}

int main()
{
    std::string start, goal;
    size_t n, size;

    std::cout << "N = ";
    std::cin >> n;

    //generate input info
    size = 2 * n + 1; //both sets of frogs + 1 empty space

    size_t i = 0;
    for (; i < n; i++)
    {
        start += 'L';
        goal += 'R';
    }

    start += '_';
    goal += '_';
    i++;

    for (; i < size; i++)
    {
        start += 'R';
        goal += 'L';
    }

    Node* first = new Node{ start };

    generateTree(first, size);

    //printTree(first);
    //std::cout << "\n\n";

    std::vector<std::string> path = findGoal(first, size, goal);

    for (size_t i = path.size() - 1; i > 0; --i)
    { 
        std::cout << path[i] << '\n';
    }
    std::cout << path[0] << '\n';
}
