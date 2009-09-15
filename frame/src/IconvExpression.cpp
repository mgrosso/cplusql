#ident "file_id $Id: IconvExpression.cpp,v 1.2 2005/01/26 20:36:56 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      IconvExpression.cpp
// Author:    mgrosso 
// Created:   Sun Dec 12 17:58:26 EST 2004 on upto11
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 
// 
// $Id: IconvExpression.cpp,v 1.2 2005/01/26 20:36:56 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "IconvExpression.h"
#include "BaseException.h"
#include "ByteBuf.h"
#include <iconv.h>
#include <string.h>

class IconvExpressionPriv 
{
    private:
    friend class IconvExpression ;
    ExpressionPtr           src_;
    iconv_t                 conv_;
    ByteBufPtr              buf_;
    IconvExpressionPriv( ExpressionPtr &src, 
        const char *to, const char *from )
    :src_(src),conv_(iconv_open(to, from )),buf_(new ByteBuf(1024,1024))
    {
        if(!conv_)
        {
            PANICV("iconv_open(%s,%s) failed.", to, from );
        }
    };
    void notify()
    {
        char *raw = const_cast<char *>(src_->getRaw());
        size_t inlen = strlen(raw);
        size_t inlen_start = inlen ;
        size_t outbuflen = inlen*4;
        buf_->shrink(outbuflen);
        char *outbuf = buf_->buf();
        memset(outbuf, '\0', outbuflen );
        while(inlen)
        {
            size_t ret = iconv( conv_, &raw, &inlen, &outbuf, &outbuflen );
            if( ret == (size_t)(-1))
            {
                switch( errno )
                {
                    case EILSEQ :
                        //ignore characters we dont understand.
                        if( * ++raw && inlen )
                        {
                            --inlen;
                            continue;
                        }
                        //but stop if we're done.
                        inlen=0;
                        break;
                    case EINVAL:
                        //incomplete multichar input.  aint gonna be
                        //no more input, so silently skip the rest of 
                        //this string...
                        inlen=0;
                        break;
                    case E2BIG:
                        PANICV("4x inputbuflen really ought to be plenty "
                            "of space. inlen_start=%lu, bufsize=%lu buf "
                            "content len=%lu inlen=%lu outlen=%lu", 
                            inlen_start, 
                            buf_->bytesUsed(), buf_->currentLength(), 
                            inlen, outbuflen );
                    default:
                        PANICV( 
                            "inlen_start=%lu inlen=%lu outlen=%lu"
                            " ibufp[%s] in[%s] out[%s]\n",
                            inlen_start, inlen, outbuflen,
                            src_->getRaw(), buf_->get() 
                            );
                }
            }
        }
    };
};

IconvExpression::IconvExpression( ExpressionPtr &src, 
    const char *to, const char *from )
{
    d_=new IconvExpressionPriv (src, to, from );
}

IconvExpression::~IconvExpression()
{
    delete d_;
}


const char *
IconvExpression::getRaw() 
{
    return d_->buf_->get();
}

const char *
IconvExpression::getRaw() const 
{
    return d_->buf_->get();
}

void
IconvExpression::notify() 
{
    d_->src_->notify();
    d_->notify();    
}




