using namespace std;

template<class Type>
inline void Swap(Type& a, Type& b)
{
	Type temp = b;
	b = a;
	a = temp;
}

template<class Type>
int Partition(Type a[], const int& p, const int& r)
{
	int i = p, j = r + 1;
	Type x = a[p];
	while (true)
	{
		while (a[++i] < x && i < r);
		while (a[--j] > x);
		if (i >= j)
			break;
		Swap(a[i], a[j]);	//交换大于基准元素的元素和小于基准元素的元素
	}
	a[p] = a[j];
	a[j] = x;
	return j;
}

template<class Type>
int RandomizedPartition(Type a[], const int& p, const int& r)
{
	srand(time(0));
	int i = (rand() * 1000) % (r - p + 1) + p;	//rand()范围[0, 65536]，依据测试集大小扩大范围
	Swap(a[p], a[i]);
	return Partition(a, p, r);
}

template<class Type>
void QuickSort(Type a[], const int& p, const int& r)
{
	if(p < r)
	{
		int q = RandomizedPartition(a, p, r);
		QuickSort(a, p, q - 1);
		QuickSort(a, q + 1, r);
	}
}