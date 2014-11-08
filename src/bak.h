//
// bak.h
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

#ifndef __ENV_H
#define __ENV_H

// c

// c++
#include <string>

// boost

// local

//
namespace mti {

class bak
{
    public:
        //
        bak();
        bak( std::string fn );

        //
        ~bak() {}

        //
        std::string name();

        //
        bool save();
        
    protected:
    private:
        //
        std::string fname_;
        std::string sname_;
};

} // mti

#endif // __ENV_H
