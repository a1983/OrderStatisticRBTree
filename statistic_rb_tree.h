#ifndef STATIC_RB_TREE
#define STATIC_RB_TREE

struct RBNode {
	RBNode* p;
	RBNode* l;
	RBNode* r;

	enum Color { Red, Black	} c;

	int s;

	static RBNode* null;

	// construct null node
	RBNode() : p{ this }, l{ this }, r{ this }, c{ Black }, s{ 0 } { }

	// construct node with value
	RBNode( RBNode* p, RBNode* l, RBNode* r ) : p{ p }, l{ l }, r{ r }, c{ Red }, s{ 1 } { }
};

RBNode* nextNode( RBNode* node );
RBNode* prevNode( RBNode* node );
RBNode* lowestNode( RBNode* node );

struct RbTreeData {
	RbTreeData();

	void rotateLeft(RBNode* n);
	void rotateRight(RBNode* n);
	void rebalance(RBNode* n);
	void deleteNodeAndRebalance(RBNode* n);

	RBNode* getByStatistic( int index );
	int getStatistic( RBNode* node );

	int statistic_size() const;

	RBNode* root_;
};

template< class K, class V >
class RbTree : RbTreeData {
public:
	typedef K KeyType;
	typedef V ValueType;

	struct Node : RBNode {
		Node( K key, V val )
			: RBNode{ RBNode::null, RBNode::null, RBNode::null }
			, key{ key }, val{ val } { }
		K key;
		V val;
	};

	class iterator
	{
		Node* i;

	public:
//		typedef std::bidirectional_iterator_tag iterator_category;
//		typedef std::size_t difference_type;
		typedef V  value_type;
		typedef V* pointer;
		typedef V& reference;

		inline iterator() : i{ Node::null } { }
		inline iterator( Node* node ) : i{ node } { }

		inline const KeyType& key() const { return i->key; }
		inline KeyType& value() const { return i->val; }
		inline KeyType& operator * () const { return i->val; }
		inline KeyType* operator -> () const { return &i->val; }
		inline bool operator == (const iterator& o) const { return i == o.i; }
		inline bool operator != (const iterator& o) const { return i != o.i; }

		inline iterator &operator ++ () {	i = static_cast<Node*>( nextNode( i ) ); return *this; }
		inline iterator operator ++ (int) { iterator r = *this; i = nextNode( i ); return r; }
		inline iterator &operator -- () {	i = prevNode( i ); return *this; }
		inline iterator operator -- (int) { iterator r = *this; i = prevNode( i ); return r; }
		inline iterator operator + (int j) const { return getNth( getStatistic( i ) + j ); }
		inline iterator operator - (int j) const { return getNth( getStatistic( i ) - j ); }
		inline iterator &operator += (int j) { return *this = *this + j; }
		inline iterator &operator -= (int j) { return *this = *this - j; }
	};

	iterator begin() { return iterator{ static_cast<Node*>( lowestNode( root_ ) ) }; }
	iterator end() { return iterator{ static_cast<Node*>( RBNode::null ) }; }

	iterator insert( K key, V val );
	iterator find( K key );
	bool remove( K key );

	iterator getNth( int index );

	inline int size() const { return statistic_size(); }

	bool valid() const;
};

template< class K, class V >
typename RbTree<K,V>::iterator RbTree<K,V>::insert(K key, V val)
{
	Node* node = new Node{ key, val };

	if( root_ == RBNode::null ) {
		root_ = node;
	}
	else {
		RBNode* p;
		RBNode* find = root_;
		while( true ) {
			p = find;
			if( key < static_cast<Node*>(find)->key ) {
				find = find->l;
				if( find == RBNode::null ) {
					p->l = node;
					break;
				}
			}
			else {
				find = find->r;
				if( find == RBNode::null ) {
					p->r = node;
					break;
				}
			}
		}

		node->p = p;
	}

	rebalance( node );

	return iterator( node );
}

template< class K, class V >
RBNode* findNode( typename RbTree<K,V>::Node* find, K key )
{
	while( find != RBNode::null ) {
		if( key < find->key ) {
			find = static_cast<typename RbTree<K,V>::Node*>( find->l );
		}
		else if( key > find->key ) {
			find = static_cast<typename RbTree<K,V>::Node*>( find->r );
		}
		else
			return find;
	}

	return find;
}

template< class K, class V >
inline typename RbTree<K,V>::iterator RbTree<K,V>::find( K key )
{
	return iterator{ findNode<K,V>( static_cast<Node*>( root_ ), key ) };
}

template< class K, class V >
bool RbTree<K,V>::remove(K key)
{
	Node* find = static_cast<Node*>( findNode<K,V>( static_cast<Node*>( root_ ), key ) );
	if( find == RBNode::null )
		return false;

	deleteNodeAndRebalance( find );
	return true;
}

template< class K, class V >
inline typename RbTree<K,V>::iterator RbTree<K,V>::getNth( int index )
{
	return iterator{ static_cast< RbTree<K,V>::Node* >( getByStatistic( index ) ) };
}

#if CHECK_VALID == 0
#include <assert.h>
#include <stack>

template< class K, class V >
int leftBlackHeight( typename RbTree<K,V>::Node* n );

template< class K, class V >
int rightBlackHeight( typename RbTree<K,V>::Node* n );

template< class K, class V >
int leftBlackHeight( RBNode* n ) {
	assert( n != RBNode::null );

	RBNode* l = n->l;
	if( l == RBNode::null )
		return 1;

	assert( ( static_cast<typename RbTree<K,V>::Node*>( l )->key )
			<= ( static_cast<typename RbTree<K,V>::Node*>( n )->key ) );

	int leftHeight = leftBlackHeight<K,V>( l );
	int rightHeight = rightBlackHeight<K,V>( l );
	assert( leftHeight == rightHeight );
	int h = leftHeight;
	if( l->c == RBNode::Black )
		return h + 1;
	return h;
}

template< class K, class V >
int rightBlackHeight( RBNode* n ) {
	assert( n != RBNode::null );

	RBNode* r = n->r;
	if( r == RBNode::null )
		return 1;

	assert( ( static_cast<typename RbTree<K,V>::Node*>( r )->key )
			>= ( static_cast<typename RbTree<K,V>::Node*>( n )->key ) );

	int leftHeight = leftBlackHeight<K,V>( r );
	int rightHeight = rightBlackHeight<K,V>( r );
	assert( leftHeight == rightHeight );
	int h = leftHeight;
	if( r->c == RBNode::Black )
		return h + 1;
	return h;
}

template< class K, class V >
bool RbTree<K,V>::valid() const
{
	assert( RBNode::null->c == RBNode::Black
		&& RBNode::null->l == RBNode::null->r
		&& RBNode::null->l == RBNode::null->p
		&& RBNode::null->l == RBNode::null );

	if( root_ == RBNode::null )
		return true;

	int leftHeight = leftBlackHeight<K,V>( root_ );
	int rightHeight = rightBlackHeight<K,V>( root_ );
	assert( leftHeight == rightHeight );

	return leftHeight == rightHeight;
}
#endif


#endif // define STATIC_RB_TREE