#ifndef HUFFMAN_TREE_H_
#define HUFFMAN_TREE_H_

#include <vector>
#include <queue>

using namespace std;

template<class Type>
struct BinaryTree
{
	Type value;
	BinaryTree<Type>* left_child;
	BinaryTree<Type>* right_child;
};

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

#endif  //HUFFMAN_TREE_H_

template<class Type>
HuffmanTree<Type>::HuffmanTree(const int& n, const Type* freq)
{
	tree_ = new BinaryTree<int>[2 * n - 1];
	vector<HuffmanNode> node(n);
	for (int i = 0; i < n; i++)
	{
		tree_[i + n - 1] = { i, nullptr, nullptr };
		node[i] = { &tree_[i + n - 1], freq[i] };
	}
	priority_queue<HuffmanNode, vector<HuffmanNode>, greater<HuffmanNode>>
		min_heap(node.begin(), node.end());
	int cur_index = n - 2;
	HuffmanNode x, y;
	for (int i = 1; i < n; i++)
	{
		x = min_heap.top();
		min_heap.pop();
		y = min_heap.top();
		min_heap.pop();
		tree_[cur_index] = { -1, x.tree, y.tree };
		x = { &tree_[cur_index], x.weight + y.weight };
		min_heap.push(x);
		cur_index--;
	}
}


template <class Type>
const Type GetAverageLen(BinaryTree<int>* tree, const Type* freq, const int& depth)
{
	if (tree->value != -1)
		return freq[tree->value] * depth;
	return GetAverageLen(tree->left_child, freq, depth + 1) +
		GetAverageLen(tree->right_child, freq, depth + 1);
}

template <class Type>
const Type HuffmanTree<Type>::GetAverageCodeLen(const Type* freq)
{
	return GetAverageLen<Type>(tree_, freq, 0);
}

