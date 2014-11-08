//
// env.cpp
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

// c
#include <stdlib.h>
#include <string.h>
#include <time.h>

// c++
#include <iostream>
#include <fstream>

// boost

// local
#include "bak.h"
#include "exp.h"
#include "ver.h"

using namespace std;

//
namespace mti {

//
bak::bak() : fname_( "" ), sname_( "" )
{
    // manual mode, do nothing 
}

//
bak::bak( std::string fn ) : fname_( fn ), sname_( "" )
{
    //
    if ( ! save() )
        throw exp ( "Could not save backup of file [" + fname_ + "]: " + name(), EXP_ERROR );
}

//
std::string bak::name()
{
    //
    if ( sname_.length() == 0 )
    {
        //
        if ( ( fname_.length() > 0 ) && ( fname_ != "-" ) )
            sname_ = fname_ + "." + stamp();
    }

    //
    return sname_;
}

//
bool bak::save()
{
    //
    bool ok = false;

    //
    if ( ( fname_.length() > 0 ) && ( fname_ != "-" ) )
    {
        //
        if ( ::exists( fname_ ) )
        {
            //
            if ( sname_.length() == 0 )
                name();

            //
            if ( ! ::exists( sname_ ) )
            {
                //
                ifstream fi( fname_.c_str(), ifstream::binary );
                ofstream fo( sname_.c_str(), ofstream::binary );

                //
                fo << fi.rdbuf();

                //
                fo.close();
                fi.close();

                //
                ok = true;
            }
            else
                ok = false;
        }
        else
            ok = true;
    }
    else
        ok = true;

    //
    return ok;
}

} // mti
