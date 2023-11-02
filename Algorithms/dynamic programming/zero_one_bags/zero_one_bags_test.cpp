#include <iostream>
#include <fstream>
#include <time.h>
#include "improved_knapsack.cpp"

using namespace std;

int main()
{
    ifstream in("bag.in", ios::in);
    if (!in.is_open())
        exit(-1);
    int c, n;
    in >> c >> n;
    int* w = new int[n + 1];
    int* v = new int[n + 1];
    int* x = new int[n + 1];
    for (int i = 1; i <= n; i++)
    {
        in >> w[i] >> v[i];
    }
    in.close();

    ofstream out("bag.out", ios::out);

    clock_t start, end;
    start = clock();
    out << Knapsack<int>(n, c, v, w, x) << endl;
    end = clock();
    cout << "time: " << end - start;

    for (int i = 1; i <= n; i++)
    {
        out << x[i];
    }
    out.close();
    delete[] w;
    delete[] v;
    delete[] x;
}

