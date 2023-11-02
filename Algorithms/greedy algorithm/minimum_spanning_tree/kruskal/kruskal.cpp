#include <fstream>
#include <map>
#include <queue>

using namespace std;

class node {
public:
    int u, v, w;

    node(int a = 0, int b = 0, int c = 0) 
        :u(a), v(b), w(c) {}

    bool operator < (const node& b) const {
        return this->w > b.w;
    }
};

class D_Set { // 并查集
private:
    int* parent;

public:
    D_Set(const int& n) 
    {
        parent = new int[n];
        for (int i = 0; i < n; i++) 
            parent[i] = i; 
    }

    ~D_Set()
    {
        delete[] parent;
    }


    int Find(int x) {
        if (parent[x] == x)
            return x;
        int p = Find(parent[x]);
        parent[x] = p;
        return p;
    }

    int Union(int x, int y) {
        int xr = Find(x);
        int yr = Find(y);
        if (xr != yr)
            parent[yr] = xr;
        return xr;
    }

    bool InSameSet(int x, int y) {
        return Find(x) == Find(y);
    }
};

int kruskal(priority_queue<node>& Q) {
    int W = 0;

    D_Set dset(Q.size());
    node n;

    while (!Q.empty()) {
        n = Q.top(); Q.pop();
        if (dset.InSameSet(n.u, n.v))
            continue;

        dset.Union(n.u, n.v);
        W += n.w;
    }

    return W;
}

int main() 
{
    ifstream fin("kruskal.in", ios::in);
    int n, e;
    fin >> n >> e;
    priority_queue<node> Q;
    for(int i = 0; i < e; i++)
    {
        int u, v, w;
        fin >> u >> v >> w;
        Q.push(node(u, v, w));
    }

    ofstream fout("kruskal.out", ios::out);
    fout << kruskal(Q);

    return 0;
}
