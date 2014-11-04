//
// tns2ldap.h
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

#ifndef __TNS2LDAP_H
#define __TNS2LDAP_H

// c

// c++
#include <string>

// boost

// local
#include "ver.h"
#include "tns.h"

#define UTIL_APP    "tns2ldap"

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

    void print();

    protected:
    private:
        //
        std::string tnsfile_;
        std::string ldpfile_;
        bool        ok_;

        //
        mti::tnsnames tns_;

        //
        bool options( int c, char** v );
};

} // mti

#endif // __TNS2LDAP_H

