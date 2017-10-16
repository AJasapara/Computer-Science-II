// MyMap.h

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap() {
		m_size = 0;
		root = nullptr;
	}
	~MyMap() { clear(); }
	void clear() { // recursively deletes tree
		deleteHelper(root);
	}

	int size() const { return m_size; }
	void associate(const KeyType& key, const ValueType& value) {
		if (root == nullptr) { // if map is empty
			root = new Node(key, value, nullptr, nullptr);
			m_size++;
			return;
		}
		Node* found = recursiveFind(key, root); // if key already exists
		if (found != nullptr)
			found->value = value;
		else {
			Node* above = nodeAbove(key, root, nullptr); // adds new node if key does not exist
			if (above != nullptr) {
				if (key < above->key)
					above->left = new Node(key, value, nullptr, nullptr);
				else
					above->right = new Node(key, value, nullptr, nullptr);
			}
			m_size++;
		}
	}

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const {
		Node* found = recursiveFind(key, root); // recursively finds key
		if (found != nullptr) {
			return &found->value;
		}
		return nullptr;
	}

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node { // basic node allowing for 2 children
		KeyType key;
		ValueType value;
		Node* left;
		Node* right;
		Node(KeyType k, ValueType v, Node* l, Node* r) {
			key = k;
			value = v;
			left = l;
			right = r;
		}
	};
	Node *root;
	int m_size;
	Node* recursiveFind(const KeyType& key, Node* head) const { // logarithmic search goes left or right based on key
		if (head == nullptr)
			return nullptr;
		Node* found = head;
		if (key < head->key)
			found = recursiveFind(key, head->left);
		else if (key > head->key)
			found = recursiveFind(key, head->right);
		else
			return head;
		return found;

	}

	Node* nodeAbove(const KeyType& key, Node* head, Node* parent) const { // returns nodeAbove desired node
		if (head == nullptr)
			return parent;
		if (key < head->key)
			return nodeAbove(key, head->left, head);
		return nodeAbove(key, head->right, head);
	}

	void deleteHelper(Node* head) { // recursively deletes tree
		if (head == nullptr)
			return;
		deleteHelper(head->left);
		deleteHelper(head->right);
		delete head;
	}
};
