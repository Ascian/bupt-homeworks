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
void RecursiveMergeSort(Type lst1[], Type lst2[], const int& left, const int& right)
{
	if (left < right)
	{
		int middle = (left + right) / 2;
		RecursiveMergeSort(lst1, lst2, left, middle);
		RecursiveMergeSort(lst1, lst2, middle + 1, right);
		Merge(lst2, lst1, left, middle, right);
		for (int i = left; i <= right; i++)
			lst1[i] = lst2[i];	
	}
}

template<class Type>
void MergeSort(Type lst[], const int& n)
{
	Type* temp = new Type[n];	//避免每递归调用一次，申请一次辅助数组
	RecursiveMergeSort(lst, temp, 0, n - 1);
	delete(temp);
}