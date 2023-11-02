using namespace std;

typedef struct range
{
	int left, right; //左闭右开
}range;

template<class Type>
inline void Swap(Type& a, Type& b)
{
	Type temp = b;
	b = a;
	a = temp;
}

template<class Type>
range Partition(Type a[], const int& p, const int& r)
{
	//划分为三路，第一路小于小于基准元素，第二路等于基准元素，第三路大于基准元素
	int lt = p, i = p + 1, gt = r + 1;	//lt指向第一路的最右边，i指向第二路的最右边，gt指向第三路的最左边
	Type x = a[p];
	while (i < gt)
	{
		if (a[i] < x)
		{
			Swap(a[i], a[lt + 1]);
			i++;
			lt++;
		}
		else if (a[i] > x)
		{
			Swap(a[i], a[gt - 1]);
			gt--;
		}
		else
			i++;
	}
	a[p] = a[lt];
	a[lt] = x;
	return { lt, gt };
}

template<class Type>
range RandomizedPartition(Type a[], const int& p, const int& r)
{
	srand(time(0));
	int i = (rand() * 1000) % (r - p + 1) + p;	//rand()范围[0, 65536]，依据测试集大小扩大范围
	Swap(a[p], a[i]);
	return Partition(a, p, r);
}

template<class Type>
void QuickSort(Type a[], const int& p, const int& r)
{
	if (p < r)
	{
		range q = RandomizedPartition(a, p, r);
		QuickSort(a, p, q.left - 1);
		QuickSort(a, q.right, r);
	}
}