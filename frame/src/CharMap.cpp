#ident "file_id $Id: CharMap.cpp,v 1.1 2004/03/03 17:21:25 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CharMap.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb 23 22:55:18 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: CharMap.cpp,v 1.1 2004/03/03 17:21:25 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "CharMap.h"
#include "CharIndex.h"
#include "limits.h"

#define MAX_KEY UCHAR_MAX

template class< T >
class CharMapNode
{
    friend class CharMap<T>;
    friend class CharMapPriv<T>;
private:
    T                   value_;
    CharMapNode<T>*     children_;
    unsigned            len_;
    bool                assigned_value_ ;


                        ~CharMapNode();
                        CharMapNode();
                        CharMapNode( const T &v );

    CharMapNode<T> *    next( int k );
    CharMapNode<T> *    find( unsigned char *key, unsigned len, CharIndex *ci );

    void                resize( unsigned len );
    CharMapNode<T> *    addif( int k, bool &didadd );
    CharMapNode<T> *    addif( int k, const T &v, bool &didadd );
    CharMapNode<T> *    findOrAdd( unsigned char *key, unsigned len, 
                            const T &v, const CharIndex *ci, bool &didadd  );

    bool                rm( unsigned char *key, unsigned len, 
                            const CharIndex *ci ); 
    bool                empty(); 
};

template class< T >
class CharMapPriv 
{
    friend class CharMap<T> ;
    friend class CharMapNode<T> ;
private:
    CharIndex           indices_[MAX_KEY];
    CharMapNode<T> *    root_;
    bool                leak_ ;
                        CharMapPriv();
                        ~CharMapPriv();
    void                checklen( unsigned len );
};

template class< T >
CharMapPriv<T>::CharMapPriv()
{
    root_=new CharMapNode();
    leak_=false;
};

template class< T >
CharMapPriv<T>::~CharMapPriv()
{
    delete root_;
};

template class< T >
void
CharMapPriv<T>::checklen( unsigned len )
{
    if( len > MAX_KEY )
    {
        PANICV( "cant insert key longer than %i", MAX_KEY );
    }
}


template class< T >
CharMapNode<T>::CharMapNode()
:children_(NULL),len_(0),value_(),assigned_value_(false)
{
    ;//noop
}

template class< T >
CharMapNode<T>::CharMapNode( const T &v )
:children_(NULL),len_(0),value_(v),assigned_value_(true)
{
    ;//noop
}

template class< T >
CharMapNode<T>::~CharMapNode()
{
    if( children_ )
    {
        delete [] children_;
        children_=NULL;
    }
}

template class< T >
CharMapNode<T> *
CharMapNode<T>::next( int k )
{
    if( k == -1 || k >= len_ )
    {
        return NULL;
    }
    return children_[k];
}

template class< T >
CharMapNode<T> *
CharMapNode<T>::find( const unsigned char *key, unsigned len, const CharIndex *ci  )
{
    CharMapNode<T> *n=this;
    for( unsigned i=0; n && ( i < len ); ++i  )
    {
        n = n->next( ci->getIndex( key[i] ));
    }
    return n;
}

template class< T >
void
CharMapNode<T>::resize( unsigned i )
{
    if( i <= len_ )
    {
        return;
    }
    CharMapNode<T> * copy=children_;
    children_=new CharMapNode<T> * [ i ];
    unsigned j ;
    for( j=0; j < len_ ; ++j )
    {
        children_[j] = copy[j];
    }
    for( ; j < i ; ++j )
    {
        children_[j] = NULL ;
    }
    delete [] copy ;
}

template class< T >
CharMapNode<T> *
CharMapNode<T>::addif( int k, bool &didadd )
{
    if( k < 0 )
    {
        PANICV( "CharMapNode<T>::addif( int k ): k < 0 ");
    }
    resize( k + 1 );
    if( ! children_[k] )
    {
        children_[k] == new CharMapNode<T> ();
        didadd=true;
    }
    return children_[k];
}

template class< T >
CharMapNode<T> *
CharMapNode<T>::addif( int k, const T &v, bool &didadd )
{
    if( k < 0 )
    {
        PANICV( "CharMapNode<T>::addif( int k ): k < 0 ");
    }
    resize( k + 1 );
    if( ! children_[k] )
    {
        children_[k] = new CharMapNode<T> ( v );
        didadd=true;
    }
    else if( ! children_[k].assigned_value_ )
    {
        children_[k].value_ = v ;
        children_[k].assigned_value_ = true ;
        didadd=true;
    }
    return children_[k];
}

template class< T >
CharMapNode<T> *    
CharMapNode<T>::findOrAdd( const unsigned char *key, unsigned len, 
        const T &v , const CharIndex *ci, bool &didadd  )
{
    CharMapNode<T> *n=this;
    didadd = false;
    for( 
            unsigned i=0; 
            n && ( i < len - 1 ); 
            ++i, ++key  
    )
    {
        n = n->addif( ci->makeIndex( *key, didadd ));
    }
    //i == len-1, key points to last char
    return n->addif( ci->makeIndex( *key ), v, didadd );
}

template class< T >
bool
CharMapNode<T>::rm( const unsigned char *key, unsigned len, const CharIndex *ci )
{
    CharMapNode<T> *n               = this;
    int *index                      = new int[len];
    CharMapNode<T> **r              = new CharMapNode<T> *[len];
    ptr< CharMapNode<T> * > rptr    = r;
    ptr< int > indexptr             = index ;

    unsigned i ;
    for( i=0; i < len - 1 ; ++i );
    {
        r[i]=NULL;
        index[i]==-1;
    }
    for( i=0; n && ( i < len ); ++i, ++key )
    {
        r[i]=n;
        index[i]=ci->getIndex( *key );
        n = n->next( index[i] );
    }
    if( n == NULL || n->assigned_value_ == false )
    {
        return false;
    }
    //else i == len-1, and *key is last char
    n->assigned_value_=false;
    T t;
    n->value_=t;
    for( ; i >0 ; --i )
    {
        if( ! r[i]->empty() )
        {
            break;
        }
        r[i-1]->children_[ index[i-1] ] = NULL;
        delete r[i];
    }
    return true ;
}

template class< T >
bool
CharMapNode<T>::empty()
{
    if( assigned_value_ ) 
    {
        return true;
    }
    if( ! len_ )
    {
        return true;
    }
    for(unsigned j=0; j< len_ ; ++j )
    {
        if( NULL != children_[j] )
        {
            return false;
        }
    }
    //no children left, even though children_ had len_ > 0 
    return true;
}

template class< T >
unsigned long
CharMapNode<T>::find_rm( const unsigned char *key, unsigned len, 
        const CharIndex *ci, bool &didadd  )
{
    CharMapNode<T> *n=this;
    unsigned i ;
    for( i=0; n && ( i < len ); ++i  )
    {
        n = n->next( ci->getIndex( key[i] ));
    }
    if( !n )
    {
        return 0;
    }
    
    list< CharMapNode<T> * > stack ;
    stack.push_front( n );
    
    while( ! stack.empty() )
    {
        CharMapNode<T> * front = stack.front();
        stack.pop_front();
        for( i=0; i< front->len_ ; ++i )
        {
            if( front->children_[i] )
            {
                stack.push_front( front->children_[i] );
                front->children_[i] = NULL ;
            }
        }
        delete front;
    }
}

template class< T >
CharMap<T>::CharMap()
{
    d_=new CharMapPriv ();
}

template class< T >
CharMap<T>::~CharMap()
{
    static const char * na = "";
    if( ! d_->leak_ )
    {
        d_->root_->find_rm( static_cast< const unsigned_char *>(na), 0 );
    }
    delete d_;
}

template class< T >
void
CharMap<T>::set_leak_mem( bool b )
{
    d_->leak_=b;
}

template class< T >
bool
CharMap<T>::insertCreate( const unsigned char *key, unsigned len, const T &value )
{
    d_->checklen( len );
    bool didadd=false;
    (void)  d_->root_->findOrAdd( key, len, value, d_->indices_, didadd );
    return didadd;
}

template class< T >
bool
CharMap<T>::insertUnique( const unsigned char *key, unsigned len, T &value )
{
    d_->checklen( len );
    bool didadd=false;
    CharMapNode<T> * node= 
        d_->root_->findOrAdd( key, len, value, d_->indices_, didadd );

    if( ! didadd )
    {
        value = node->value_;
    }
    return didadd;
}

template class< T >
bool
CharMap<T>::insertReplace( const unsigned char *key, unsigned len, T &value )
{
    d_->checklen( len );
    bool didadd=false;
    CharMapNode<T> * node= 
        d_->root_->findOrAdd( key, len, value, d_->indices_, didadd );
    if( ! didadd )
    {
        node->value_ = value ;
        return true;
    }
    return false;
}

template class< T >
bool
CharMap<T>::haveKey( const unsigned char *key, unsigned len )
{
    d_->checklen( len );
    return NULL == d_->root_->find( key, len, d_->indices_ ) ?  false : true ;
}

template class< T >
bool
CharMap<T>::readValue( const unsigned char *key, unsigned len, T &value )
{
    d_->checklen( len );
    bool didadd=false;
    CharMapNode<T> * node= 
        d_->root_->find( key, len, d_->indices_ );
    if( node )
    {
        value = node->value_;
        return true;
    }
    return false;
}

template class< T >
bool
CharMap<T>::rm( const unsigned char *key, unsigned len )
{
    d_->checklen( len );
    return d_->root_->rm( key, len, d_->indices_ );
}

template class< T >
unsigned long 
CharMap<T>::find_rm( const unsigned char *key, unsigned len )
{
    d_->checklen( len );
    return d_->root_->find_rm( key, len, d_->indices_ );
}

