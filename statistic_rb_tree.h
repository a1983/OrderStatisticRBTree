#ifndef STATIC_RB_TREE
#define STATIC_RB_TREE

#include <iterator>

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

RBNode* getNodeByOrder( RBNode* root, int order );
int getNodeOrder( RBNode* node );

RBNode* getDistanceNode( RBNode* node, int distance );


class RBTreeData {

    template< class K, class V, class C >
    friend class OrderStatisticTree;

    RBTreeData();

    void rotateLeft(RBNode* n);
    void rotateRight(RBNode* n);
    void rebalance(RBNode* n);
    RBNode *removeNodeAndRebalance(RBNode* n);

    int getStatistic( RBNode* node );

    int statisticSize() const;

    RBNode* root_;
};

template< class K, class V, class Comparer = std::less< K > >
class OrderStatisticTree : RBTreeData {

    struct Node : RBNode {
        Node( const K& key, const V& val )
            : RBNode{ RBNode::null, RBNode::null, RBNode::null }
            , key{ key }, val{ val } { }
        K key;
        V val;
    };

    int blackHeight( RBNode* n ) const;
    Node* findNode( Node* find, const K& key );

    inline static Node* cast( RBNode* node ) { return static_cast< Node* >( node ); }

public:
    typedef K KeyType;
    typedef V ValueType;

    class iterator
    {
        friend class OrderStatisticTree;

        Node* i;

    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::size_t difference_type;
        typedef V  value_type;
        typedef V* pointer;
        typedef V& reference;

        inline iterator() : i{ Node::null } { }
        inline explicit  iterator( Node* node ) : i{ node } { }

        inline const KeyType& key() const { return i->key; }
        inline KeyType& value() const { return i->val; }
        inline int order() const { return getNodeOrder( i ); }

        inline KeyType& operator * () const { return i->val; }
        inline KeyType* operator -> () const { return &i->val; }
        inline bool operator == (iterator o) const { return i == o.i; }
        inline bool operator != (iterator o) const { return i != o.i; }

        inline iterator &operator ++ ()
            { i = static_cast<Node*>( nextNode( i ) ); return *this; }
        inline iterator operator ++ (int)
            { iterator r = *this; i = static_cast<Node*>( nextNode( i ) ); return r; }

        inline iterator &operator -- ()
            { i = static_cast<Node*>( prevNode( i ) ); return *this; }
        inline iterator operator -- (int)
            { iterator r = *this; i = static_cast<Node*>( prevNode( i ) ); return r; }

        inline iterator operator + (int j) const
            { return iterator { static_cast<Node*>( getDistanceNode( i,  j ) ) }; }
        inline iterator operator - (int j) const
            { return iterator { static_cast<Node*>( getDistanceNode( i, -j ) ) }; }

        inline iterator &operator += (int j) { return *this = *this + j; }
        inline iterator &operator -= (int j) { return *this = *this - j; }
    };

    class const_iterator
    {
        const Node* i;

    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef std::size_t difference_type;
        typedef V  value_type;
        typedef V* pointer;
        typedef V& reference;

        inline const_iterator() : i{ Node::null } { }
        inline explicit  const_iterator( const Node* node ) : i{ node } { }

        inline const KeyType& key() const { return i->key; }
        inline const KeyType& value() const { return i->val; }
        inline int order() const { return getNodeOrder( i ); }

        inline KeyType& operator * () const { return i->val; }
        inline KeyType* operator -> () const { return &i->val; }
        inline bool operator == (const_iterator o) const { return i == o.i; }
        inline bool operator != (const_iterator o) const { return i != o.i; }

        inline const_iterator &operator ++ ()
            { i = static_cast<Node*>( nextNode( const_cast<Node*>( i ) ) ); return *this; }
        inline const_iterator operator ++ (int)
            { iterator r = *this; i = static_cast<Node*>( nextNode( i ) ); return r; }

        inline const_iterator &operator -- ()
            { i = static_cast<const Node*>( prevNode( i ) ); return *this; }
        inline const_iterator operator -- (int)
            { iterator r = *this; i = static_cast<Node*>( prevNode( i ) ); return r; }

        inline const_iterator operator + (int j) const
            { return iterator { static_cast<Node*>( getDistanceNode( i,  j ) ) }; }
        inline const_iterator operator - (int j) const
            { return iterator { static_cast<Node*>( getDistanceNode( i, -j ) ) }; }

        inline const_iterator &operator += (int j) { return *this = *this + j; }
        inline const_iterator &operator -= (int j) { return *this = *this - j; }
    };


    explicit OrderStatisticTree( const Comparer& comparer = Comparer() )
        : lessThan_( comparer )
    { }

    iterator begin() { return iterator{ static_cast<Node*>( lowestNode( root_ ) ) }; }
    iterator end() { return iterator{ static_cast<Node*>( RBNode::null ) }; }

    const_iterator begin() const
        { return const_iterator{ static_cast<Node*>( lowestNode( root_ ) ) }; }
    const_iterator end() const
        { return const_iterator{ static_cast<Node*>( RBNode::null ) }; }

    iterator insertMulti( const K& key, const V& val );
    iterator find( const K& key );
    iterator erase( iterator i );

    bool removeOne( const K& key );
    int removeMulti( const K& key );

    iterator getNth( int order );

    inline int size() const { return statisticSize(); }

    void clear();

    bool valid() const;

private:
    Comparer lessThan_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

template< class K, class V, class C >
typename OrderStatisticTree< K, V, C >::iterator
OrderStatisticTree< K, V, C >::insertMulti( const K& key, const V& val )
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
            if( lessThan_( key, cast( find )->key ) ) {
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


template< class K, class V, class C >
typename OrderStatisticTree< K, V, C >::Node*
OrderStatisticTree< K, V, C >::findNode( typename OrderStatisticTree< K, V, C >::Node* n,
                                         const K& key )
{
    while( n != RBNode::null ) {
        if( lessThan_( key, n->key ) )       { n = cast( n->l ); }
        else if( lessThan_( n->key, key ) )  { n = cast( n->r ); }
        else                                 return n;
    }

    return n;
}

template< class K, class V, class C >
inline typename OrderStatisticTree< K, V, C >::iterator
OrderStatisticTree< K, V, C >::find( const K& key )
{
    return iterator{ findNode( cast( root_ ), key ) };
}

template< class K, class V, class C >
inline typename OrderStatisticTree< K, V, C >::iterator
OrderStatisticTree< K, V, C >::erase( typename OrderStatisticTree< K, V, C >::iterator i ) {
    iterator next{ cast( nextNode( i.i ) ) };
    if( i.i != Node::null )
        delete cast( removeNodeAndRebalance( i.i ) );
    return next;
}

template< class K, class V, class C >
bool OrderStatisticTree< K, V, C >::removeOne( const K& key )
{
    Node* find = findNode( static_cast<Node*>( root_ ), key );
    if( find == RBNode::null )
        return false;

    delete cast( removeNodeAndRebalance( find ) );

    return true;
}

template< class K, class V, class C >
int OrderStatisticTree< K, V, C >::removeMulti( const K& key )
{
    int removed = 0;
    Node* find = findNode( root_, key );
    while( find != RBNode::null ) {
        Node* next = nextNode( findNode( find, key ) );
        delete cast( removeNodeAndRebalance( find ) );
        find == next;
    }

    return removed;
}

template< class K, class V, class C >
inline typename OrderStatisticTree< K, V, C >::iterator
OrderStatisticTree< K, V, C >::getNth( int order )
{
    return iterator{ cast( getNodeByOrder( root_, order ) ) };
}

template< class NodeType >
void deleteNodeRecursively( NodeType* node ) {
    if( node->l != RBNode::null )
        deleteNodeRecursively( static_cast<NodeType*>( node->l ) );

    if( node->r != RBNode::null )
        deleteNodeRecursively( static_cast<NodeType*>( node->r ) );

    delete node;
}

template< class K, class V, class C >
void OrderStatisticTree< K, V, C >::clear()
{
    if( root_ != RBNode::null )
        deleteNodeRecursively( cast( root_ ) );

    root_ = Node::null;
}

#if CHECK_VALID == 0
#include <assert.h>

template< class K, class V, class C >
int OrderStatisticTree< K, V, C >::blackHeight( RBNode* n ) const {
    assert( n != RBNode::null );

    Node* l = cast( n->l );
    Node* r = cast( n->r );

    int leftHeight = 0;
    int rightHeight = 0;
    if( l != RBNode::null ) {
        leftHeight += blackHeight( l );
        assert( !lessThan_( cast( n )->key, l->key ) );
    }

    if( r != RBNode::null ) {
        rightHeight += blackHeight( r );
        assert( !lessThan_( r->key, cast( n )->key ) );
    }

    assert( leftHeight == rightHeight );
    return n->c == RBNode::Black ? leftHeight + 1 : leftHeight;
}

template< class K, class V, class C >
bool OrderStatisticTree< K, V, C >::valid() const
{
    assert( RBNode::null->c == RBNode::Black
            && RBNode::null->l == RBNode::null->r
            && RBNode::null->l == RBNode::null->p
            && RBNode::null->l == RBNode::null );

    if( root_ == RBNode::null )
        return true;

    return blackHeight( root_ ) > 0;
}
#endif


#endif // define STATIC_RB_TREE
