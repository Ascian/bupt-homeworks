#include <cmath>

template<class Type>
void Trackback(int n, Type* w, Type* v, Type (*pt)[2], int* head, int* x)
{
	Type j = pt[head[0] - 1][0], m = pt[head[0] - 1][1];	//m为容量c
	for (int i = 0; i <= n; i++)
	{
		x[i] = 0;
		for (int k = head[i + 1]; k <= head[i] - 1; k++)
		{
			if (pt[k][0] + w[i] == j && pt[k][1] + v[i] == m)	//跳跃点(j, m)为函数m(i+1, j-w[i])+v[i]的跳跃点
			{
				x[i] = 1;
				j = pt[k][0];
				m = pt[k][1];
				break;
			}
		}
	}
}

template<class Type>
Type Knapsack(int n, Type c, Type* v, Type* w, int* x, bool is_int = true)
{
	int size = pow(2, n + 1) - 1;
	if (is_int)	//n为整数时，只需要min(n(c + 1), 2^n+1 - 1)大小的辅助空间
	{
		if (size <= 0 || n * (c + 1) < size)
			size = n * (c + 1);
	}		
	Type(*pt)[2] = new Type[size][2];

	int* head = new int[n + 2];	//指向跳跃点集p[i]起始点的位置
	head[n + 1] = 0;
	pt[0][0] = 0;
	pt[0][1] = 0;
	//设置left和right分别指向上一个已求得点集p[i+1]的起始点和最后一个点的位置
	//next指向当前点集p[i]存放下一个点的位置
	int left = 0, right = 0, next = 1;	
	head[n] = 1;	
	//依次求出p[n],p[n-1],...,p[1]
	for (int i = n; i >= 1; i--)	
	{
		int k = left;
		//遍历函数m(i+1, j-w[i])+v[i]的跳跃点，并将非受控点加入新点集p[i]
		for (int j = left; j <= right; j++)	
		{
			if (pt[j][0] + w[i] > c)
				break;
			Type y = pt[j][0] + w[i], m = pt[j][1] + v[i];
			//加入新跳跃点左侧的所有跳跃点到当前点集
			while (k <= right && pt[k][0] < y)	
			{
				pt[next][0] = pt[k][0];
				pt[next++][1] = pt[k++][1];
			}
			//若新跳跃点处有着m值更大的跳跃点，则新跳跃点为受控点
			if (k <= right && pt[k][0] == y)	
			{
				if (m < pt[k][1])
					m = pt[k][1];
				k++;
			}
			//若新跳跃点不为受控点将其加入当前点集
			if (m > pt[next - 1][1])	
			{
				pt[next][0] = y;
				pt[next++][1] = m;
			}
			//跳过新加入点右侧的所有受控点
			while (k <= right && pt[k][1] <= pt[next - 1][1])
				k++;
			//此时k指向第一个不受控于新跳跃点的跳跃点
		}
		//将剩余非受控点加入新点集
		while (k <= right)
		{
			pt[next][0] = pt[k][0];
			pt[next++][1] = pt[k++][1];
		}
		left = right + 1;
		right = next - 1;
		head[i - 1] = next;
	}
	Trackback<Type>(n, w, v, pt, head, x);
	Type result = pt[next - 1][1];
	delete[] pt;
	delete[] head;
	return result;
}

