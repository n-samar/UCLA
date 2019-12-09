// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.
#ifndef MYMAP_H
#define MYMAP_H
#include <iostream>

using namespace std;



template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap();
	~MyMap();
	void clear();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;
	int getDepth(){return getDepth(m_root);}
	void printMe(){printMe(m_root);}		// TODO: Delete this
private:
	struct Node
	{
		Node(KeyType key, ValueType value)
		: m_key(key),
		m_value(value),
		m_lChild(nullptr),
		m_rChild(nullptr) {}

		KeyType m_key;
		ValueType m_value;
		Node* m_lChild;
		Node* m_rChild;
		KeyType getKey() const {return m_key;}
		ValueType getValue() const {return m_value;}
		const ValueType* getValuePtr() const {return &m_value;}
	};
	int getDepth(Node* root)
	{
		if(root == nullptr) return 0;
		return 1+max(getDepth(root->m_lChild), getDepth(root->m_rChild));
	}
		void printMe(Node* curr)
	{
		if(curr == nullptr) return;

		printMe(curr->m_lChild);
		printMe(curr->m_rChild);
	}
	// adds node to tree in proper position
	// returns false if the key in n is already
	// in the tree, true otherwise.
	bool addNode(Node* n);	

	// Delete all node elements of the tree rooted at root
	void cleanTree(Node* root);
	Node* m_root;
	int m_size;
};



template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap() 
{
	m_root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::~MyMap() 
{
	clear();
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear() 
{
	cleanTree(m_root);
	m_root = nullptr;
	m_size = 0;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const 
{
	return m_size;
}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) 
{
	Node* n = new Node(key, value);
	if(addNode(n)) m_size++;
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType, ValueType>::find(const KeyType& key) const 
{
	const Node* currPtr = m_root;

	// traverse tree until you reach a value equal to the searched or
	// a nullptr
	while (currPtr != nullptr && currPtr->getKey() != key)
	{
		if (key < currPtr->getKey()) 
			currPtr = currPtr->m_lChild;
		else
			currPtr = currPtr->m_rChild;
	}
	if(currPtr == nullptr) return nullptr;
	else if(currPtr->getKey() == key)
		return (currPtr->getValuePtr());

	return nullptr;		// This code should be unreachable!
}

template<typename KeyType, typename ValueType>
bool MyMap<KeyType, ValueType>::addNode(Node* n)
{
		// When adding to empty map
	if(m_root == nullptr)
	{
		m_root = n;
		return true;
	}

	Node* currPtr = m_root;
	if(m_root->m_key == n->m_key)
	{
		n->m_lChild = m_root->m_lChild;
		n->m_rChild = m_root->m_rChild;
		delete m_root;
		m_root = n;
		return false;
	}
	// traverse tree until you reach a value equal to the searched or
	// a nullptr
	while (currPtr != nullptr)
	{
		bool flag = true;
		if (n->getKey() < currPtr->getKey())
		{
			if(currPtr->m_lChild == nullptr
				|| currPtr->m_lChild->getKey() == n->getKey())
			{
				if(currPtr->m_lChild != nullptr)
				{
					flag = false;
					n->m_lChild = currPtr->m_lChild->m_lChild;
					n->m_rChild = currPtr->m_lChild->m_rChild;
					delete currPtr->m_lChild;
				}
				currPtr->m_lChild = n;
				return flag;
			}
			currPtr = currPtr->m_lChild;
		}else
		{
			if(currPtr->m_rChild == nullptr
				|| currPtr->m_rChild->getKey() == n->getKey())
			{
				if(currPtr->m_rChild != nullptr)
				{
					flag = false;
					n->m_lChild = currPtr->m_rChild->m_lChild;
					n->m_rChild = currPtr->m_rChild->m_rChild;
					delete currPtr->m_rChild;
				}
				currPtr->m_rChild = n;
				return flag;
			}
			currPtr = currPtr->m_rChild;
		}
	}


	return false;	

}

template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::cleanTree(Node* root)
{
	if(root == nullptr) return;
	cleanTree(root->m_rChild);
	cleanTree(root->m_lChild);
	delete root;
}
#endif	// MYMAP_H