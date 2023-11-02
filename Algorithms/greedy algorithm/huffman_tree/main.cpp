#include <fstream>
#include <queue>
#include <vector>
#include <iomanip>

#include "huffman_tree.h"

using namespace std;

int main()
{
    ifstream fin("huffman.in", ios::in);
    int n;
    fin >> n;
    double* freq = new double[n];
    for (int i = 0; i < n; i++)
    {
        double f;
        fin >> f;
        freq[i] = f;
    }
    fin.close();

    HuffmanTree<double> huffman_tree(n, freq);

    ofstream fout("huffman.out", ios::out);
    fout << setiosflags(ios::fixed) << setprecision(3) << huffman_tree.GetAverageCodeLen(freq);
    fout.close();
    delete[] freq;
}