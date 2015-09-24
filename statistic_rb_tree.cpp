#include "statistic_rb_tree.h"

RBNode* RBNode::null = new RBNode();

RBTreeData::RBTreeData()
    : root_{ RBNode::null }
{

}

RBNode* nextNode(RBNode* node)
{
    if( node->r != RBNode::null ) {
        node = node->r;
        while( node->l != RBNode::null )
            node = node->l;
    }
    else {
        RBNode* p = node->p;
        while( p != RBNode::null && node == p->r ) {
            node = p;
            p = node->p;
        }
        node = p;
    }
    return node;
}


RBNode* prevNode(RBNode* node)
{
    if( node->l != RBNode::null ){
        node = node->l;
        while( node->r != RBNode::null )
            node = node->r;
    }
    else {
        RBNode* p = node->p;
        while( p != RBNode::null && node == p->l ) {
            node = p;
            p = node->p;
        }
        node = p;
    }
    return node;
}


RBNode* lowestNode( RBNode* node)
{
    RBNode* find = node;
    while( find != RBNode::null ) {
        node = find;
        find = node->l;
    }
    return node;
}

RBNode* getNodeByOrder( RBNode* root, int order )
{
    RBNode* find = root;
    int current = 0;
    while( find != RBNode::null ) {
        int check = current + find->l->s;
        if( order < check ) {
            find = find->l;
        }
        else if( order > check ) {
            current = check + 1;
            find = find->r;
        }
        else {
            return find;
        }

    }
    return find;
}

int getNodeOrder( RBNode* node )
{
    RBNode* find = node;
    RBNode* p = node->p;
    int order = 0;
    while( p != RBNode::null ) {
        if( find == p->r ) {
            order = p->s + 1;
        }
        find = p;
        p = p->p;
    }
    return order + node->l->s;
}

RBNode* getDistanceNode( RBNode* node, int distance )
{
    RBNode* find = node;
    RBNode* p = node->p;
    int order = 0;
    while( p != RBNode::null ) {
        if( find == p->r ) {
            order = p->s + 1;
        }
        find = p;
        p = p->p;
    }

    // now find is root node
    order = order + node->l->s + distance;
    return getNodeByOrder( find, order );
}

void RBTreeData::rotateLeft(RBNode* n)
{
    RBNode* r = n->r;
    n->r = r->l;
    if( r->l != RBNode::null )
        r->l->p = n;

    r->p = n->p;

    if( n == root_ )		root_ = r;
    else if( n == n->p->l ) n->p->l = r;
    else                    n->p->r = r;

    r->l = n;
    n->p = r;

    // statistic counting
    n->s = n->l->s + n->r->s + 1;
    r->s = r->l->s + r->r->s + 1;
}


void RBTreeData::rotateRight(RBNode* n)
{
    RBNode* l = n->l;
    n->l = l->r;
    if( l->r != RBNode::null )
        l->r->p = n;

    l->p = n->p;

    if( n == root_ )		root_ = l;
    else if( n == n->p->r ) n->p->r = l;
    else                    n->p->l = l;

    l->r = n;
    n->p = l;

    // statistic counting
    n->s = n->l->s + n->r->s + 1;
    l->s = l->l->s + l->r->s + 1;
}

void RBTreeData::rebalance(RBNode* n)
{
    // statistic counting
    for( RBNode* p = n->p; p != RBNode::null; ++p->s, p = p->p ){ };

    while( n != root_ && n->p->c == RBNode::Red ) {
        if( n->p == n->p->p->l ) {
            RBNode* u = n->p->p->r;
            if( u != RBNode::null && u->c == RBNode::Red ) {
                n->p->c = RBNode::Black;
                u->c = RBNode::Black;
                n->p->p->c = RBNode::Red;
                n = n->p->p;
            } else {
                if( n == n->p->r ) {
                    n = n->p;
                    rotateLeft( n );
                }
                n->p->c = RBNode::Black;
                n->p->p->c = RBNode::Red;
                rotateRight( n->p->p );
            }
        } else {
            RBNode *u = n->p->p->l;
            if( u != RBNode::null && u->c == RBNode::Red ) {
                n->p->c = RBNode::Black;
                u->c = RBNode::Black;
                n->p->p->c = RBNode::Red;
                n = n->p->p;
            } else {
                if( n == n->p->l ) {
                    n = n->p;
                    rotateRight( n );
                }
                n->p->c = RBNode::Black;
                n->p->p->c = RBNode::Red;
                rotateLeft( n->p->p );
            }
        }
    }
    root_->c = RBNode::Black;
}

RBNode* RBTreeData::removeNodeAndRebalance(RBNode* n)
{
    // removing
    RBNode* old = n;
    RBNode *to;
    RBNode *to_parent;

    if( old->l == RBNode::null ) {
        to = old->r;
    }
    else {
        if( old->r == RBNode::null ) {
            to = old->l;
        }
        else {
            old = old->r;
            while( old->l != RBNode::null )
                old = old->l;
            to = old->r;
        }
    }

    if( old != n ) {
        n->l->p = old;
        old->l = n->l;
        if( old != n->r ) {
            to_parent = old->p;
            if( to != RBNode::null )
                to->p = old->p;
            old->p->l = to;
            old->r = n->r;
            n->r->p = old;
        }
        else {
            to_parent = old;
        }

        if( root_ == n )		root_ = old;
        else if( n->p->l == n )	n->p->l = old;
        else                    n->p->r = old;
        old->p = n->p;
        // Swap the colors
        RBNode::Color c = old->c;
        old->c = n->c;
        n->c = c;
        old = n;
    }
    else {
        to_parent = old->p;
        if( to != RBNode::null )
            to->p = old->p;
        if( root_ == n )        root_ = to;
        else if( n->p->l == n ) n->p->l = to;
        else                    n->p->r = to;
    }

    // statistic counting
    for( RBNode* p = to_parent; p != RBNode::null; p->s = p->l->s + p->r->s + 1, p = p->p ){ };

    // recolor
    if( old->c == RBNode::Black ) {
        while( to != root_ && ( to->c == RBNode::Black ) ) {
            if( to == to_parent->l ) {
                RBNode* w = to_parent->r;
                if( w->c == RBNode::Red ) {
                    w->c = RBNode::Black;
                    to_parent->c = RBNode::Red;
                    rotateLeft( to_parent );
                    w = to_parent->r;
                }

                if( w->l->c == RBNode::Black && w->r->c == RBNode::Black ) {
                    w->c = RBNode::Red;
                    to = to_parent;
                    to_parent = to_parent->p;
                }
                else {
                    if( w->r->c == RBNode::Black ) {
                        if( w->l != RBNode::null )
                            w->l->c = RBNode::Black;
                        w->c = RBNode::Red;
                        rotateRight( w );
                        w = to_parent->r;
                    }
                    w->c = to_parent->c;
                    to_parent->c = RBNode::Black;
                    if( w->r != RBNode::null )
                        w->r->c = RBNode::Black;
                    rotateLeft( to_parent );
                    break;
                }
            }
            else {
                RBNode *w = to_parent->l;
                if( w->c == RBNode::Red ) {
                    w->c = RBNode::Black;
                    to_parent->c = RBNode::Red;
                    rotateRight( to_parent );
                    w = to_parent->l;
                }

                if( w->r->c == RBNode::Black && w->l->c == RBNode::Black ) {
                    w->c = RBNode::Red;
                    to = to_parent;
                    to_parent = to_parent->p;
                }
                else {
                    if( w->l->c == RBNode::Black ) {
                        if( w->r != RBNode::null )
                            w->r->c = RBNode::Black;
                        w->c = RBNode::Red;
                        rotateLeft( w );
                        w = to_parent->l;
                    }
                    w->c = to_parent->c;
                    to_parent->c = RBNode::Black;
                    if( w->l != RBNode::null )
                        w->l->c = RBNode::Black;
                    rotateRight( to_parent );
                    break;
                }
            }
        }

        if( to != RBNode::null )
            to->c = RBNode::Black;
    }

    return old;
}

int RBTreeData::statisticSize() const
{
    return root_->s;
}
