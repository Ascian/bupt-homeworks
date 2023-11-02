# 贪心算法

## 哈夫曼编码

### 实验题目
既定编码字符集$C$及其频率分布$f$，即C中的任意字符C以频率f(c)在数据文件中出现。C的一个前缀码编码方案对应一棵二叉树T。字符c在树T中的深度记为$d_T(c)$。$d_T(c)$也是c的前缀码长。该编码方案的平均码长定义为

$$ B(T) = \sum_{c \in C}f(c)d_T(c) $$

使平均码长达到最小的前缀码编码方案成为C的最优前缀码。

哈夫曼提出了构造最优前缀码的贪心算法，产生的编码方案称为哈夫曼算法。

问题：对给出的字符集设计哈夫曼编码，并求出平均码长。

输入文件名为`huffman.in`。输入共两行。
第一行一个正整数 n，代表字符个数。
第二行包含n个三位小数P_i，代表第i个字符的出现概率，两数之间用空格隔开。

输出文件名为`huffman.out`。输出共一行。
第一行包含一个三位小数W，为最后的期望。


### 实验过程

算法以|C|个叶结点开始，执行|C|-1次的合并运算后最终产生所要求的的树。

下面给出算法的具体描述：

首先，定义哈夫曼树类

    template<class Type>
    class HuffmanTree
    {
    public:
        //生成哈夫曼树
        //parameter
        //    const int& n - 字符数量
        //    const Type* freq - 字符出现频率
        HuffmanTree(const int& n, const Type* freq);

        ~HuffmanTree()
        {
            delete[] tree_;
        }

        //获取平均码长
        //parameter
        //    const Type* freq - 字符出现频率
        const Type GetAverageCodeLen(const Type* freq);

    private:
        struct HuffmanNode
        {
            bool operator>(const HuffmanNode& node) const
            {
                return weight > node.weight;
            }
            BinaryTree<int>* tree;
            Type weight;
        };

        //哈夫曼树，tree_[0]为根节点
        BinaryTree<int>* tree_; 
    };

构造函数获取字符出现频率，生成哈夫曼树，函数`const Type GetAverageCodeLen(const Type* freq)`用于获取平均码长。数据成员`tree_`保存哈夫曼树。

下面详解生成哈夫曼树的算法。

首先初始化哈夫曼树，将叶子结点存入树中。叶子结点存放在[n-1: 2*n-2]的位置上

	tree_ = new BinaryTree<int>[2 * n - 1];
	vector<HuffmanNode> node(n);
	for (int i = 0; i < n; i++)
	{
		tree_[i + n - 1] = { i, nullptr, nullptr };
		node[i] = { &tree_[i + n - 1], freq[i] };
	}

接着初始化优先队列

    priority_queue<HuffmanNode, vector<HuffmanNode>, greater<HuffmanNode>>
        min_heap(node.begin(), node.end());

接着反复合并最小频率的两棵二叉树，将合并后的节点存入哈夫曼树中。存放位置从`tree_[n]`开始向序号小的方向直到`tree_[0]`，`tree_[0]`作为整棵哈夫曼树的根节点。

    int cur_index = n - 2;
	HuffmanNode x, y;
	for (int i = 1; i < n; i++)
	{
        //获取最小频率的两棵二叉树
		x = min_heap.top();
		min_heap.pop();
		y = min_heap.top();
		min_heap.pop();
        //将合并后的根节点存入哈夫曼树中
		tree_[cur_index] = { -1, x.tree, y.tree };
		x = { &tree_[cur_index], x.weight + y.weight };
		min_heap.push(x);
		cur_index--;
	}


哈夫曼编码算法用优先队列`priority_queue`实现最小堆。初始化优先队列需要O(n)计算时间，由于最小堆的pop和push均需要O(logn)时间，n-1次的合并共需要O(nlogn)计算时间。因此，关于n个字符的哈夫曼编码算法的时间复杂度为O(nlogn)。由于需要最小堆需要O(n)的空间和二叉树需要O(2n-1)的空间，因此算法的空间复杂度为O(n)级别。

## 单源最短路径

### 实验题目

给定一个带权有向图G = (V, E)，其中每条边的权时是负实数。另外，给定V中的一个定点称为源。现在要计算从源到所有其他各顶点的最短路径。这里路的长度时指路上各边权值和。这个问题通常称为单源最短路径问题。

问题：求出给定顶点 1 与顶点 V 之间的最短路。

输入文件名为 `dijkstra.in`。
第一行两个正整数 V 、E，分别代表顶点数、边数。
接下来 E 行包含三个正整数 u、v、w，代表 u、v 之间存在一条权值为 w 的无向边。

输出文件名为 `dijkstra.out`。输出共一行。
第一行包含一个整数为最短路上权值之和，若顶点 1 与顶点 V 不连通，输出-1。

### 实验过程

Dijkstra算法时解决单源最短路径的一个贪心算法。其基本思想是，设置顶点集合S，并不断地做贪心选择来扩充这个集合。一个定点属于集合S当且仅当从源到该顶点的最短路径长度已知。初始时，S中仅含有源。设u是G的某个顶点，把从源到u且中间仅经过S中顶点的路称为从源到u的特殊路径，并用数组dist记录当前每个顶点所对应的最短路径长度。算法每次冲V-S中取出具有最短特殊路径长度的顶点u，并将u加入到S中，同时对数组dist做必要的修改。一旦S包含了所有V中的顶点，dist就记录了从源到所以其他地方顶点之间的最短路径长度。

由于G为无向图，因此可以采用下三角矩阵存储图，而无需邻接矩阵。以下为下三角矩阵类定义：

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


以下为具体的dijkstra算法描述：

首先需要定义表示是否加入集合S的数组，并将起点加入S。之后初始化dist数组，dist设置为从起点只经过S中结点到达各点的距离。

    bool* s = new bool[c.size()];  //表示是否加入集合S的数组
    //初始化dist
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

之后反复查询不在集合S中且从起点只经过S中结点到达各点的最小距离，即dist最小的结点，设这个点为u。将u加入集合S后，修改因u加入导致的dist数组的改变。直到所有点到加入到集合S中

    for (int i = 0; i < c.size(); i++)
    {
        //查找不在集合S且dist最小的结点
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
        s[u] = true;  //将不在集合S且dist最小的结点加入S
        //修改因u加入导致的dist数组的改变
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


对于一个具有n个顶点和e条边的带权无向图，算法的主体循环需要O(n)时间。这个循环需要执行n-1次，所以完成循环需要O(n^2)时间。同时，存储下三角矩阵需要O(n/2)的空间，存储每个点到起点的最短距离数组dist和路径前一个节点的数组prev都需要O(n)空间，因此算法空间复杂度为O(n)。

## 最小生成树

### 实验题目

设G=(V, E)时无向连通带权图，即一个网络。E中每条边(v, w)的权威c[v][w]。如果G的一个子图G'设计一棵包含G的所有顶点的树，则称G'为G的生成树。生成树上各边权的总和称为树的耗费。在G的所有生成树中，耗费最小的生成树称为G的最小生成树。

最小生成树的性质：如果$(u, v) \in E$，且$u \in U$，$v \in V$，且在所有这样的边中，(u, v)的权c[u][v]最小，那么一定存在G的一棵最小生成树，它以(u, v)为其中的一条边。

问题：分别使用Prim算法和Kruskal算法计算给定无向图上以顶点 1 为根的最小生成树边权和。

输入文件名为 `prim.in`和`kruskal.in`。
第一行包含两个正整数 V 、E，分别代表顶点数与边数。
接下来 E 行包含三个正整数 u、v、w，代表 u、v 之间存在一条权值为 w 的无向边。

输出文件名为 `prim.out`和`kruskal.out`。输出共一行。
第一行包含一个整数代表最小生成树的权值和。

### 实验步骤

#### Prim算法

由于G为无向图，因此同样可以采用下三角矩阵存储图，而无需邻接矩阵

为了有效找出满足条件$j \in S$, $j \in V-S$，且权c[i][j]最小的边。实现这个目的的简单办法是，设置两个数组closest和lowcost。

    int* closest = new int[n + 1];  //closest[j]是，j在S中距离最近的结点
    Type* lowcost = new Type[n + 1];  //lowcost[j]是j到closest[j]的距离

首先值S={1}，然后，只要S是V的真子集，就做如下贪心选择：选取满足条件$ i \in S $，$ j \in V-S $，且c[i][j]最小的边，并将顶点j加入到S中。这个过程一直进行到S=V时为止，选取到的所有边恰好构成G的一棵最小生成树。

以下为具体的Prim算法描述：

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

#### Kruskal算法

首先将G的n个顶点看成n个独立的连通分支，将所有边按权从大到小排序；然后从第一条边开始，依次权递增的顺序查看每条边，并按下述方法连接两个同步的连通分支：当查看到第k条边(v, w)时，如果端点v和w分别是当前两个不同的连通分支$T_1$和$T_2$中的顶点时，就用边(v, w)将$T_1$和$T_2$连接成一个连通分支，然后继续查看第k+1条边；如果端点v和w在同一个分支中，就接着查看第k+1条边。直到只剩下一个连通分支为止。

以下为具体的算法描述：

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

当图的变数为e时，Kruskal算法所需的时间是O(eloge)。当e= $\Omega (n^2)$时，Kruskal算法比Prim算法差，但当e = $O(n^2)$时，Kruskal算法要比Prim算法好很多。

### 算法源代码

详见源代码部分
