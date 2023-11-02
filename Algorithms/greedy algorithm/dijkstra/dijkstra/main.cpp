#include <fstream>
#include <vector>

using namespace std;

//下三角矩阵
template<class Type>
class LowerTriangularMatrix
{
public:
    LowerTriangularMatrix(const int& n, const Type& default_value)
        :num_(n), matrix_(new Type* [n])
    {
        for (int i = 0; i < n; i++)
        {
            matrix_[i] = new Type[i];
            for (int j = 0; j < i; j++)
                matrix_[i][j] = default_value;
        }
    }
    ~LowerTriangularMatrix()
    {
        for (int i = 0; i < num_; i++)
        {
            delete[] matrix_[i];
        }
        delete[] matrix_;
    }

    const Type& at(const int& x, const int& y) const
    {
        if (x < y)
            return matrix_[y][x];
        return matrix_[x][y];
    }

    void set(const int& x, const int& y, const Type& value)
    {
        if (x < y)
            matrix_[y][x] = value;
        else
            matrix_[x][y] = value;
    }

    const int& size() const
    {
        return num_;
    }

private:
    int num_;
    Type** matrix_;
};

template<class Type>
void Dijkstra(Type* dist, int* prev, const int& v, const LowerTriangularMatrix<Type>& c)
{
    bool *s = new bool[c.size()]; // 表示是否加入集合S的数组
    // 初始化dist
    for (int i = 0; i < c.size(); i++)
    {
        dist[i] = c.at(v, i);
        s[i] = false;
        if (dist[i] == INT_MAX)
            prev[i] = 0;
        else
            prev[i] = v;
    }
    dist[v] = 0;
    s[v] = true;
    for (int i = 0; i < c.size(); i++)
    {
        // 查找不在集合S且dist最小的结点
        int temp = INT_MAX;
        int u = v;
        for (int j = 0; j < c.size(); j++)
        {
            if ((!s[j]) && (dist[j] < temp))
            {
                u = j;
                temp = dist[j];
            }
        }
        s[u] = true; // 将不在集合S且dist最小的结点加入S
        // 修改因u加入导致的dist数组的改变
        for (int j = 0; j < c.size(); j++)
        {
            if ((!s[j]) && (c.at(u, v) < INT_MAX))
            {
                Type newdist = dist[u] + c.at(u, v);
                if (newdist < dist[j])
                {
                    dist[j] = newdist;
                    prev[j] = u;
                }
            }
        }
    }
    delete[] s;   
}

int main()
{
    ifstream fin("dijkstra.in", ios::in);
    int n, e;
    fin >> n >> e;
    LowerTriangularMatrix<int> c(n, INT_MAX);
    for (int i = 0; i < e; i++)
    {
        int u, v, w;
        fin >> u >> v >> w;
        c.set(u - 1, v - 1, w);
    }
    fin.close();

    ofstream fout("dijkstra.out", ios::out);
    int* dist = new int[n];
    int* prev = new int[n];
    Dijkstra(dist, prev, 0, c);
    fout << dist[n - 1];
    fout.close();
    delete[] dist;
    delete[] prev;
    return 0;
}

