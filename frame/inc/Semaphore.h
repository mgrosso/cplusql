// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Semaphore.h
// Author:    mgrosso 
// Created:   Thu Mar  1 16:38:15 EST 2001 on dev1.int.primaryknowledge.com
// Project:   
// Purpose:   os independent semaphore class. should work for linux and solaris
// 
// Copyright (c) 2001 Matt Grosso
// 
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// $Id: Semaphore.h,v 1.6 2004/05/27 21:57:49 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SEMAPHORE_H
#define SEMAPHORE_H 1
class Semaphore
{
private:
    void *s_;
public:
    Semaphore();
    Semaphore(int value);
    ~Semaphore();
    int wait();
    int trywait();
    int post();
    void getvalue( int *v );
};

#endif /* SEMAPHORE_H */
