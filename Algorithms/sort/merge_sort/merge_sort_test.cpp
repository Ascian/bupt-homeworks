#include <iostream>
#include <fstream>
#include <time.h>
#include "recursive_merge_sort.cpp"

using namespace std;

int main()
{
    fstream fin("mergesort.in", ios::in), fout("mergesort.out", ios::out);
    int n;
    fin >> n;
    int* lst = new int[n];

    for (int i = 0; i < n; i++)
        fin >> lst[i];

    clock_t start, end;
    start = clock();
    MergeSort<int>(lst, n);
    end = clock();
    cout << "time: " << end - start;

    fout << lst[0];
    for (int i = 1; i < n; i++)
        fout << " " << lst[i];

    delete(lst);
    return 0;
}

