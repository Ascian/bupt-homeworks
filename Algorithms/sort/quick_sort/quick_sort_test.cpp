#include <iostream>
#include <fstream>
#include <time.h>
#include "threeway_quick_sort.cpp"

using namespace std;

int main()
{
    fstream fin("quicksort.in", ios::in), fout("quicksort.out", ios::out);
    int n;
    fin >> n;
    int* lst = new int[n];

    for (int i = 0; i < n; i++)
        fin >> lst[i];

    clock_t start, end;
    start = clock();
    QuickSort<int>(lst, 0, n - 1);
    end = clock();
    cout << "time: " << end - start;

    fout << lst[0];
    for (int i = 1; i < n; i++)
        fout << " " << lst[i];

    delete(lst);
    return 0;
}

