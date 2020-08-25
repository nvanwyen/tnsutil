//
// tnsformat.h
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2004-2013 Metasystems Technologies Inc. (MTI)
// All rights reserved
//
// Distributed under the MTI Software License, Version 0.1.
//
// as defined by accompanying file MTI-LICENSE-0.1.info or
// at http://www.mtihq.com/license/MTI-LICENSE-0.1.info
//

#ifndef __TNSFORMAT_H
#define __TNSFORMAT_H

// c

// c++
#include <string>

// boost

// local
#include "ver.h"
#include "tns.h"

#define UTIL_APP    "tnsformat"

// generic usage
//
void usage();

// entry
//
int main( int argc, char** argv );

//
namespace mti {

// wrapper class
//
class app
{
    public:
        app( int c, char** v );
        ~app() {}

    bool ok() { return ok_; }

    int run();

    protected:
    private:
        //
        bool ok_;

        //
        std::string tnsfile_;

        //
        bool sort_;
        bool backup_;
        bool stdout_;

        //
        mti::tnsnames tns_;

        //
        bool options( int c, char** v );
};

} // mti

//
#endif // __TNSFORMAT_H
