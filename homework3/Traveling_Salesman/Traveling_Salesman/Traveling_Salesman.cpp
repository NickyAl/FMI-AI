#include <iostream>
#include <random>
#include <time.h>
#include <cmath>
#include <vector>
#include <queue>

int SIZE;
int GENSIZE;
int GENERATIONS;

double distanceAtoB(double xA, double yA, double xB, double yB)
{
    return sqrt(pow((xA - xB), 2) + pow((yA - yB), 2));
}

struct Path
{
    std::vector<int> path;
    double distance;

    Path()
    {
        for (int i = 0; i < SIZE; i++)
            path.push_back(i);

        randomize();
    }

    Path(std::vector<int> path, double distance)
    {
        for (int i = 0; i < SIZE; i++)
        {
            this->path.push_back(path[i]);
        }

        this->distance = distance;
    }

    Path(std::vector<int> father, std::vector<int> mother)
    {
        int brkPoint1 = rand() % SIZE;
        int brkPoint2 = rand() % SIZE;

        if (brkPoint1 > brkPoint2)
        {
            int temp = brkPoint1;
            brkPoint1 = brkPoint2;
            brkPoint2 = brkPoint1;
        }

        std::queue<int> fHalf, mHalf;

        for (int i = brkPoint1; i <= brkPoint2; i++)
        {
            mHalf.push(mother[i]);
            for (int j = 0; j < SIZE; j++)
            {
                if (father[j] == mother[i])
                    father[j] = -1;
            }
        }

        for (int i = 0; i < SIZE; i++)
        {
            if(father[i] != -1)
                fHalf.push(father[i]);
        }

        for (int i = 0; i < SIZE; i++)
        {
            if (i < brkPoint1)
            {
                path.push_back(fHalf.front());
                fHalf.pop();
            }
            else if (i <= brkPoint2)
            {
                path.push_back(mHalf.front());
                mHalf.pop();
            }
            else
            {
                path.push_back(fHalf.front());
                fHalf.pop();
            }
        }
    }

    void mutate()
    {
        if (rand() % 2)
        {
            return;
        }

        switch (rand() % 5)
        {
        case 0:
            swapMutate();
            break;

        case 1:
            reverseMutate();
            break;

        case 2:
            swapMutate();
            reverseMutate();
            break;

        case 3:
            mutateStartingPoint();
            break;

        case 4:
            mutateStartingPoint();
            reverseMutate();
            break;

        default:
            break;
        }
    }

    void calcDist(double** map)
    {
        distance = 0;
        for (int i = 0; i < SIZE - 1; i++)
        {
            distance += distanceAtoB(map[path[i]][0], map[path[i]][1], map[path[i + 1]][0], map[path[i + 1]][1]);
        }
    }

    bool isNotInPath(int index)
    {
        for (std::vector<int>::iterator it = path.begin(); it != path.end(); it++)
        {
            if (*it = index)
                return false;
        }

        return true;
    }

    void print()
    {
        for (std::vector<int>::iterator it = path.begin(); it != path.end(); it++)
        {
                std::cout << *it << ' ';
        }

        std::cout << " - " << distance;
    }

private:
    void randomize()
    {
        int rndIndex;
        for (int i = 0; i < SIZE; i++)
        {
            rndIndex = rand() % SIZE;

            if (rndIndex != i)
            {
                int temp = path[i];
                path[i] = path[rndIndex];
                path[rndIndex] = temp;
            }
        }
    }

    void swapMutate()
    {
        int repeats = rand() % (SIZE / 3);
        while (repeats)
        {
            int i = rand() % SIZE;
            int j = rand() % SIZE;

            if (i == j)
                continue;

            int temp = path[i];
            path[i] = path[j];
            path[j] = temp;

            repeats--;
        }
    }

    void reverseMutate()
    {
        int i = rand() % SIZE;
        int j = rand() % SIZE;

        if (i == j)
            return;

        int temp;
        if (i > j)
        {
            temp = i;
            i = j;
            j = temp;
        }

        while (i < j)
        {
            temp = path[i];
            path[i] = path[j];
            path[j] = temp;
            i++;
            j--;
        }
    }

    void mutateStartingPoint()
    {
        int i = rand() % SIZE;

        if (i == 0)
            return;

        int temp = path[i];
        path[i] = path[0];
        path[0] = temp;
    }
};

void search(double** map)
{
    std::vector<Path> firstGen;

    for (int i = 0; i < GENSIZE; i++)
    {
        firstGen.push_back(Path()); //the first generation will have random genes
    }

    for (int i = 0; i < GENSIZE; i++)
    {
        firstGen[i].calcDist(map);
    }

    double dstncAvg2Times = 0;

    for (int i = 0; i < GENSIZE; i++)
    {
        dstncAvg2Times += firstGen[i].distance;
    }

    dstncAvg2Times = (dstncAvg2Times / GENSIZE) * 2;

    double sumOfAll = 0;
    for (int i = 0; i < GENSIZE; i++)
    {
        sumOfAll += dstncAvg2Times - firstGen[i].distance;
    }

    int older = GENSIZE / 10;
    if (older == 0)
        older++;

    for (int genIndex = 0; genIndex < GENERATIONS; genIndex++)
    {
        std::vector<Path> secondGen;
        std::queue<int> q;
        for (int i = 0; i < GENSIZE; i++)
            q.push(i);

        for (int k = 0; k < GENSIZE; k++)
        {
            if (k < older)
            {
                int min = 0;
                std::queue<int> newQ = q;

                while (!q.empty())
                {
                    if (firstGen[min].distance > firstGen[q.front()].distance)
                        min = q.front();

                    q.pop();
                }

                while (!newQ.empty())
                {
                    if (newQ.front() != min)
                        q.push(newQ.front());

                    newQ.pop();
                }

                secondGen.push_back(Path(firstGen[min].path, firstGen[min].distance));
                continue;
            }

            int randPercent = (rand() % 1000) + 1;
            int father = 0, mother = 0;

            for (int i = 0; i < GENSIZE; i++)
            {
                randPercent -= ((dstncAvg2Times - firstGen[i].distance) / sumOfAll) * 1000;
                if (randPercent < 0)
                {
                    father = i;
                    randPercent = (rand() % 1000) + 1 - ((dstncAvg2Times - firstGen[i].distance) / sumOfAll) * 1000;

                    break;
                }
            }

            for (int i = 0; i < GENSIZE; i++)
            {
                if (i == father)
                    continue;

                randPercent -= ((dstncAvg2Times - firstGen[i].distance) / sumOfAll) * 1000;
                if (randPercent < 0)
                {
                    mother = i;
                    break;
                }
            }

            Path child(firstGen[father].path, firstGen[mother].path);
            child.mutate();

            child.calcDist(map);

            secondGen.push_back(child);
        }

        firstGen = secondGen;

        if (genIndex + 1 == GENERATIONS || genIndex % SIZE == 1)
        {
            int min = 0;
            for (int i = 0; i < GENSIZE; i++)
            {
                if (firstGen[min].distance > firstGen[i].distance)
                    min = i;
            }

            firstGen[min].print();
            std::cout << '\n';
        }
    }
}


int main()
{
    srand((unsigned)time(NULL));
    std::cin >> SIZE;
    GENSIZE = SIZE * 2;
    GENERATIONS = SIZE * 10;

    double** map = new double* [SIZE];

    for (int i = 0; i < SIZE; i++)
    {
        map[i] = new double[2];
    }

    //Random map
    for (int i = 0; i < SIZE; i++)
    {
        map[i][0] = (double)rand() / 100 * (rand() % 2 ? -1 : 1);
        map[i][1] = (double)rand() / 100 * (rand() % 2 ? -1 : 1);
    }

    //Custom map
    /*map[0][0] = 0.190032E-03;
    map[0][1] = -0.285946E-03;
    map[1][0] = 383.458;
    map[1][1] = -0.608756E-03;
    map[2][0] = -27.0206;
    map[2][1] = -282.758;
    map[3][0] = 335.751;
    map[3][1] = -269.577;
    map[4][0] = 69.4331;
    map[4][1] = -246.780;
    map[5][0] = 168.521;
    map[5][1] = 31.4012;
    map[6][0] = 320.350;
    map[6][1] = -160.900;
    map[7][0] = 179.933;
    map[7][1] = -318.031;
    map[8][0] = 492.671;
    map[8][1] = -131.563;
    map[9][0] = 112.198;
    map[9][1] = -110.561;
    map[10][0] = 306.320;
    map[10][1] = -108.090;
    map[11][0] = 217.343;
    map[11][1] = -447.089;*/

    //Show map coordinates
    //for (int i = 0; i < SIZE; i++)
    //{
    //    std::cout << "(" << map[i][0] << ", " << map[i][1] << ")\n";
    //}
    //std::cout << '\n';

    search(map);
}