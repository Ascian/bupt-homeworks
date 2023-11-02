template<class Type>
void Trackback(Type** m, int* w, int c, int n, int* x)
{
	for (int i = 1; i < n; i++)
	{
		if (m[i][c] == m[i + 1][c])
			x[i] = 0;
		else
		{
			x[i] = 1;
			c -= w[i];
		}
	}
	x[n] = (m[n][c]) ? 1 : 0;
}

template<class Type>
Type Knapsack(int n, int c, Type* v, int* w, int* x)
{
	Type** m = new Type* [n + 1];
	for (int i = 1; i <= n; i++)
		m[i] = new Type[c + 1];
	int jMax = w[n] - 1 > c ? c : w[n] - 1;
	for (int j = 0; j <= jMax; j++)
		m[n][j] = 0;
	for (int j = w[n]; j <= c; j++)
		m[n][j] = v[n];
	for (int i = n - 1; i > 1; i--)
	{
		jMax = jMax = w[i] - 1 > c ? c : w[i] - 1;
		for (int j = 0; j <= jMax; j++)
			m[i][j] = m[i + 1][j];
		for (int j = w[i]; j <= c; j++)
			m[i][j] = m[i + 1][j] > (m[i + 1][j - w[i]] + v[i]) ? m[i + 1][j] : m[i + 1][j - w[i]] + v[i];
	}
	m[1][c] = m[2][c];
	if(c >= w[1])
		m[1][c] = m[2][c] > (m[2][c - w[1]] + v[1]) ? m[2][c] : m[2][c - w[1]] + v[1];

	Trackback<Type>(m, w, c, n, x);
	Type result = m[1][c];
	for (int i = 1; i <= n; i++)
		delete[] m[i];
	delete[] m;
	return result;
}

