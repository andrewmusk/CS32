// MyMap.h

// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

#include <iostream>

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

private:
    struct Node
    {
        
        Node(KeyType key, ValueType val)
        {
            nodeKey = key;
            nodeVal = val;
            right = nullptr;
            left = nullptr;
        }
        ~Node()
        {
            //node destructor recursive calls lower nodes
            if(left!=nullptr)
            {
                delete left;
            }
            if(right!=nullptr)
            {
                delete right;
            }
        }
        KeyType nodeKey;
        ValueType nodeVal;
        Node* right;
        Node* left;
    };
    Node* root;
    int m_size;
};

//initialiser
template<typename KeyType, typename ValueType>
MyMap<KeyType, ValueType>::MyMap()
{
    root = nullptr;
    m_size =0;
}
//destructor is same as clear
template<typename KeyType, typename ValueType>
MyMap<KeyType,ValueType>::~MyMap()
{
    clear();
}

//clear clears all by calling delete on root which is recursive
template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::clear()
{
    delete root;
}

template<typename KeyType, typename ValueType>
int MyMap<KeyType, ValueType>::size() const
{
    return m_size;
}

template<typename KeyType, typename ValueType>
const ValueType* MyMap<KeyType,ValueType>::find(const KeyType& key) const
{
    //find uses binary tree search method
    Node* ptr = root;
    if(m_size > 0)
    {
        //while there are still values
        while(ptr != nullptr)
        {
            if(key == ptr -> nodeKey)
            {
                return  &(ptr -> nodeVal);
            }
            //move to left if less
            else if(key < ptr -> nodeKey)
            {
                ptr = ptr -> left;
            }
            //move to right if more
            else if(key > ptr -> nodeKey)
            {
                ptr = ptr -> right;
            }
        }
    }
    //if nothing foud
    return nullptr;
}


template<typename KeyType, typename ValueType>
void MyMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    // if it is the first node then create new and add it
    if(root == nullptr)
    {
        root = new Node(key,value);
        m_size++;
        return;
    }
    Node* ptr = root;
    for(;;)
    {
        // if it already exists, change the value
        if(ptr -> nodeKey == key)
        {
            ptr -> nodeVal = value;
            return;
        }
        //if it is less
        else if(key < ptr -> nodeKey)
        {
            //if null then you can add it
            if(ptr -> left == nullptr)
            {
                ptr -> left = new Node(key,value);
                m_size++;
                return;
            }
            //otherwise keep going
            else
            {
                ptr = ptr -> left;
            }
        }
        // if it is greater
        else if(key > ptr -> nodeKey)
        {
            //if you are in an empty poistion, you can add
            if(ptr -> right == nullptr)
            {
                ptr -> right = new Node(key,value);
                m_size++;
                return;
            }
            //otherwise keep going
            else
            {
                ptr = ptr -> right;
            }
        }
    }
}

