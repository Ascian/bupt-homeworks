#include <cstdlib>
#include <time.h>

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
	int lt = p, i = p + 1, gt = r + 1;
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
void insert_sort(Type a[], const int& p, const int& r)
{
	for (int i = 1; i <= r; i++)
	{
		Type temp = a[i];
		int j = i;
		for (; j > 0 && a[j - 1] > temp; j--)
			a[j] = a[j - 1];
		a[j] = temp;
	}
}

template<class Type>
int Select(Type a[], const int& p, const int& r, const int& k)
{
	if (r - p < 75)
	{
		insert_sort(a, p, r);
		return p + k - 1;
	}
	for (int i = 0; i <= (r - p - 4) / 5; i++)
	{
		insert_sort(a, p + 5 * i, p + 5 * i + 4);
		Swap(a[p + 5 * i + 2], a[p + i]);
	}
	int x = Select(a, p, p + (r - p - 4) / 5, (r - p - 4) / 10); //找中位数的中位数
	Swap(a[x], a[p]);
	range i = Partition(a, p, r);
	int j = i.left - p + 1;
	if (k <= j)
		return Select(a, p, i.left, k);
	else
		return Select(a, i.right + 1, r, k - j);
}

template<class Type>
range MedianPartition(Type a[], const int& p, const int& r)
{
	int x = Select(a, p, r, (r - p + 1) / 2 + 1); //找中位数
	Swap(a[p], a[x]);
	return Partition(a, p, r);
}

template<class Type>
void QuickSort(Type a[], const int& p, const int& r)
{
	if (p < r)
	{
		range q = MedianPartition(a, p, r);
		QuickSort(a, p, q.left - 1);
		QuickSort(a, q.right, r);
	}
}