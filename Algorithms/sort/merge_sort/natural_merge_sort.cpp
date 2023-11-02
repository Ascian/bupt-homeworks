using namespace std;

template<class Type>
void Merge(Type lst1[], const Type lst2[], const int& left, const int& middle, const int& right)
{
	int i = left, j = middle + 1, k = left;
	while ((i <= middle) && (j <= right))
	{
		if (lst2[i] <= lst2[j])
			lst1[k++] = lst2[i++];
		else
			lst1[k++] = lst2[j++];
	}
	if (i > middle)
		for (; j <= right; j++)
			lst1[k++] = lst2[j];
	else
		for (; i <= middle; i++)
			lst1[k++] = lst2[i];
}

template<class Type>
void MergePass(Type lst1[], const Type lst2[], const int& s, const int& n, const int divide[], const int& gn)
{
	int i = 0;
	for (; i <= gn - 2 * s; i += 2 * s)
		Merge(lst1, lst2, divide[i], divide[i + s] - 1, divide[i + 2 * s] - 1);
	if (i + s < gn)
		Merge(lst1, lst2, divide[i], divide[i + s] - 1, n - 1);
	else if(i < gn)
		for (int j = divide[i]; j < n; j++)
			lst1[j] = lst2[j];
}

template<class Type>
void MergeSort(Type lst[], const int& n)
{
	int* divide = new int[n];
	divide[0] = 0;
	int GroupNum = 1;	//组数
	for (int i = 0; i < n - 1; i++)
		if (lst[i + 1] < lst[i])
			divide[GroupNum++] = i + 1;		//记录每一组第一个元素位置
	Type* temp = new Type[n];
	int s = 1;
	while (s < GroupNum)
	{
		MergePass(temp, lst, s, n, divide, GroupNum);
		s += s;
		MergePass(lst, temp, s, n, divide, GroupNum);
		s += s;
	}
	delete(temp);
}