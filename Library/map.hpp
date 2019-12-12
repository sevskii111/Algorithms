#include <memory>
#include <cmath>

template <typename Key, typename Value>
class Node {
private:
	typedef std::shared_ptr<Node<Key, Value>> Node_ptr;
public:
	Key key;
	Value value;
	size_t height;
	Node_ptr left;
	Node_ptr right;
	Node(Key key, Value value);
	const int8_t getBFactor(bool forceHeightFix);
	const int8_t getBFactor();
	void fixHeight();
	static size_t getNodeHeight(Node_ptr node);
};

template<typename Key, typename Value>
inline size_t Node<Key, Value>::getNodeHeight(Node_ptr node)
{
	return node ? node->height : 0;
}

template<typename Key, typename Value>
Node<Key, Value>::Node(Key key, Value value) {
	this->key = key;
	this->value = value;
	this->height = 1;
}

template<typename Key, typename Value>
void Node<Key, Value>::fixHeight()
{
	height = fmax(getNodeHeight(left), getNodeHeight(right)) + 1;
}

template<typename Key, typename Value>
const int8_t Node<Key, Value>::getBFactor(bool forceHeightFix)
{
	if (forceHeightFix) {
		fixHeight();
	}
	return (getNodeHeight(right) - getNodeHeight(left));
}

template<typename Key, typename Value>
const int8_t Node<Key, Value>::getBFactor()
{
	return (getNodeHeight(right) - getNodeHeight(left));
}


template <typename Key, typename Value>
class AVLTree {
private:
	typedef std::shared_ptr<Node<Key, Value>> Node_ptr;
	Node_ptr root;
	static Node_ptr appendTo(Node_ptr node, Key key, Value value);
	static Node_ptr rotateLeft(Node_ptr node);
	static Node_ptr rotateRight(Node_ptr node);
	static Node_ptr balance(Node_ptr node);
	static Node_ptr findMin(Node_ptr node);
	static Node_ptr removeMin(Node_ptr node);
	static Node_ptr remove(Node_ptr node, Key key);
	static Value find(Node_ptr node, Key key);
	static bool isBalanced(Node_ptr node);
public:
	void insert(Key key, Value value);
	void remove(Key key);
	Value find(Key key);
	bool isBalanced();
};


template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node_ptr AVLTree<Key, Value>::appendTo(Node_ptr node, Key key, Value value)
{
	if (!node) {
		return std::make_shared<Node<Key, Value>>(key, value);
	}
	else if (node->key == key) {
		node->value = value;
		return node;
	}
	else if (key < node->key) {
		node->left = appendTo(node->left, key, value);
	}
	else {
		node->right = appendTo(node->right, key, value);
	}
	return balance(node);
}

template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node_ptr AVLTree<Key, Value>::rotateLeft(Node_ptr node)
{
	Node_ptr q = node->right;
	node->right = q->left;
	q->left = node;
	node->fixHeight();
	q->fixHeight();
	return q;
}

template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node_ptr AVLTree<Key, Value>::rotateRight(Node_ptr node)
{
	Node_ptr q = node->left;
	node->left = q->right;
	q->right = node;
	node->fixHeight();
	q->fixHeight();
	return q;
}

template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node_ptr AVLTree<Key, Value>::balance(Node_ptr node)
{
	node->fixHeight();
	int16_t BFactor = node->getBFactor();
	if (BFactor == 2)
	{
		if (node->right->getBFactor() < 0)
			node->right = rotateRight(node->right);
		return rotateLeft(node);
	}
	else if (BFactor == -2)
	{
		if (node->left->getBFactor() > 0)
			node->left = rotateLeft(node->left);
		return rotateRight(node);
	}
	return node;
}

template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node_ptr AVLTree<Key, Value>::findMin(Node_ptr node)
{
	return node->left ? findMin(node->left) : node;
}

template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node_ptr AVLTree<Key, Value>::removeMin(Node_ptr node)
{
	if (!node->left) {
		return node->right;
	}
	node->left = removeMin(node->left);
	return balance(node);
}

template<typename Key, typename Value>
typename AVLTree<Key, Value>::Node_ptr AVLTree<Key, Value>::remove(Node_ptr node, Key key)
{
	if (!node) {
		return 0;
	}
	if (key < node->key) {
		node->left = remove(node->left, key);
	}
	else if (key > node->key) {
		node->right = remove(node->right, key);
	}
	else
	{
		Node_ptr l = node->left;
		Node_ptr r = node->right;
		if (!r) return l;
		Node_ptr min = findMin(r);
		min->right = removeMin(r);
		min->left = l;
		return balance(min);
	}

	return balance(node);
}

template<typename Key, typename Value>
Value AVLTree<Key, Value>::find(Node_ptr node, Key key)
{
	if (!node) {
		return Value();
	}
	else if (node->key == key) {
		return node->value;
	}
	else if (key < node->key) {
		return find(node->left, key);
	}
	else {
		return find(node->right, key);
	}
}

bool inFrame(int8_t num) {
	return (num >= -1 && num <= 1);
}

template<typename Key, typename Value>
bool AVLTree<Key, Value>::isBalanced(Node_ptr node)
{
	if (!node) return true;
	return isBalanced(node->left) && isBalanced(node->right) && inFrame(node->getBFactor(true));
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(Key key, Value value)
{
	{
		if (!root) {
			root = std::make_shared<Node<Key, Value>>(key, value);
		}
		else {
			root = appendTo(root, key, value);
		}
	}
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(Key key)
{
	root = remove(root, key);
}

template<typename Key, typename Value>
Value AVLTree<Key, Value>::find(Key key)
{
	return find(root, key);
}

template<typename Key, typename Value>
inline bool AVLTree<Key, Value>::isBalanced()
{
	return isBalanced(root);
}

template <typename Key, typename Value>
class Map {
private:
	AVLTree<Key, Value> tree;
public:
	void insert(Key key, Value value);
	void erase(Key key);
	Value find(Key key);
	bool isMyTreeBalanced();
};



template<typename Key, typename Value>
void Map<Key, Value>::insert(Key key, Value value)
{
	tree.insert(key, value);
}

template<typename Key, typename Value>
void Map<Key, Value>::erase(Key key)
{
	tree.remove(key);
}

template<typename Key, typename Value>
Value Map<Key, Value>::find(Key key)
{
	return tree.find(key);
}

template<typename Key, typename Value>
inline bool Map<Key, Value>::isMyTreeBalanced()
{
	return tree.isBalanced();
}
