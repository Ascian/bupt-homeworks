#include <fstream>

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
void Prim(LowerTriangularMatrix<bool>& is_choose, const int& n, const LowerTriangularMatrix<Type>& c)
{
    int* closest = new int[n + 1];  //closest[j]是，j在S中距离最近的结点
    Type* lowcost = new Type[n + 1];  //lowcost[j]是j到closest[j]的距离
    bool* s = new bool[n + 1];
    s[1] = true;  //加入第一个结点到S
    for (int i = 2; i <= n; i++)
    {
        lowcost[i] = c.at(1, i);
        closest[i] = 1;
        s[i] = false;
    }
    for (int i = 1; i < n; i++)
    {
        Type min = INT_MAX;
        int j = 1;
        //查找lowcost最小的点
        for (int k = 2; k <= n; k++)
        {
            if ((lowcost[k] < min) && (!s[k]))
            {
                min = lowcost[k];
                j = k;
            }
        }
        is_choose.set(j, closest[j], true);
        s[j] = true;  //加入lowcost最小的点到S
        //因j加入S，修改各点lowcost和closest变化
        for (int k = 2; k <= n; k++)
        {
            int a = c.at(j, k);
            int b = lowcost[k];
            if ((c.at(j, k) < lowcost[k]) && (!s[k]))
            {
                lowcost[k] = c.at(j, k);
                closest[k] = j;
            }
        }
    }
    delete[] lowcost;
    delete[] closest;
    delete[] s;
}


int main()
{
    ifstream fin("prim.in", ios::in);
    int n, e;
    fin >> n >> e;
    LowerTriangularMatrix<int> c(n + 1, INT_MAX);
    for (int i = 0; i < e; i++)
    {
        int u, v, w;
        fin >> u >> v >> w;
        c.set(u, v, w);
    }
    fin.close();

    ofstream fout("prim.out", ios::out);
    LowerTriangularMatrix<bool> is_choose(n + 1, false);;
    Prim<int>(is_choose, n, c);
    int sum_weight = 0;
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j < i; j++)
        {
            if (is_choose.at(i, j))
                sum_weight += c.at(i, j);
        }
    }

    fout << sum_weight;
    fout.close();
    return 0;
}
