#ident  "file_id $Id: test-resource.cpp,v 1.2 2004/01/05 20:06:24 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      test-resource.cpp
// Author:    Matt Grosso
// Created:   Mon Dec  7 01:15:22 EST 1998 on circe
// Project:   
// Purpose:   
// 
// Copyright (c) 1998 Matt Grosso.  All Rights Reserved.
// 
// $Id: test-resource.cpp,v 1.2 2004/01/05 20:06:24 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include <stdio.h>
#include "ResourcePool.h"

class A {
    public:
    A():x(1),y(2),z(3){ printf("A::A()\n");}
    ~A(){ printf("~A::A()\n");}
    int x,y,z;
};

class ResourceFactory < A > {
    public:
    A *makeResource(){ return new A(); }
    void reclaimResource( A *a ){ delete a; }
};

typedef ResourcePool<A> APool;
typedef ptr< A, ResourceReturner<A> > AHandle;
typedef ptr<ResourceFactory<A> > AFactory;

int main( int argc, char **argv ){
    /*
    AFactory foo( new ResourceFactory<A>());
    {
        AHandle a ( foo->makeResource(), NULL );
        a->x=1;
        a->y=2;
        a->z=3;
        printf("a  %i %i %i\n", a->x, a->y, a->z );
    }
    */
    APool::init(AFactory( new ResourceFactory<A>()), 1,1 );
    APool *rp= APool::instance();
    printf("rp curSize=%i curUsed=%i\n", 
        rp->getCurrentSize(), rp->getCurrentUsed());
    if(1){
        AHandle b=rp->getResource();
        printf("b  %i %i %i\n", b->x, b->y, b->z );
        printf("rp curSize=%i curUsed=%i\n", 
            rp->getCurrentSize(), rp->getCurrentUsed());
    }
    printf("rp curSize=%i curUsed=%i\n", 
        rp->getCurrentSize(), rp->getCurrentUsed());
    delete rp ;
    return 0;
};

