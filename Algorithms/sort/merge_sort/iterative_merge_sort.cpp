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
void MergePass(Type lst1[], const Type lst2[], const int& s, const int& n)
{
	int i = 0;
	for (; i <= n - 2 * s; i += 2 * s)
		Merge(lst1, lst2, i, i + s - 1, i + 2 * s - 1);
	if (i + s < n)
		Merge(lst1, lst2, i, i + s - 1, n - 1);
	else
		for (int j = i; j < n; j++)
			lst1[j] = lst2[j];
}

template<class Type>
void MergeSort(Type lst[], const int& n)
{
	Type* temp = new Type[n];
	int s = 1;
	while (s < n)
	{
		MergePass(temp, lst, s, n);
		s += s;
		MergePass(lst, temp, s, n);	//最终结果一定在lst中
		s += s;
	}
	delete(temp);
}