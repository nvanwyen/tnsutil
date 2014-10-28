//
// ldap2tns.h
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

#ifndef __LDAP2TNS_H
#define __LDAP2TNS_H

//
#include <string>
#include "ver.h"

#define UTIL_APP    "ldap2tns"

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
        bool options( int c, char** v );
};

} // mti

#endif // __LDAP2TNS_H

