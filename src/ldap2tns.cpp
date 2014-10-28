//
// ldap2tns.cpp
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

//
#include <iostream>
#include <vector>

//
#include "ldap2tns.h"
#include "opt.h"

//
using namespace std;
using namespace mti;

//
void usage()
{
    cout << "Usage: " << UTIL_APP << " options <tnsnames.ora>|<->\n";
    cout << "where:\n";
    cout << "   <tnsnames.ora>        : TNS Names output file\n";
    cout << "   <->                   : TNS output to stdout\n";
    cout << "options:\n";
    cout << "   [-D | --DN]     <val> : Distinguished name of the LDAP account\n";
    cout << "   [-w | --passwd| <val> : Distinguished name password\n";
    cout << "    -q | --prompt]       : Prompt for the Distinguished name password\n";
    cout << "   [-h | --host]   <val> : LDAP server host or IP\n";
    cout << "   [-p | --port]   <val> : LDAP server port number\n";
    cout << "   [-b | --base]   <val> : Base (parent) entry where entries are managed\n";
    cout << "\n";
}

//
int main( int argc, char** argv )
{
    int rc = -1;

    //
    version( UTIL_APP );
    copyright();

    //
    app eng( argc, argv );

    //
    if ( eng.ok() )
    {
        eng.print();
        rc = 0;
    }
    else
        rc = 1;

    //
    return rc;
}

//
app::app( int c, char** v )
{
    ok_ = options( c, v );
}

//
void app::print()
{
    using namespace std;

    cout << "tnsfile_ = " << tnsfile_ << "\n";
    cout << "ldpfile_ = " << ldpfile_ << "\n";
}

//
bool app::options( int c, char** v )
{
    using namespace std;
    using namespace GetOpt;

    //
    GetOpt_pp ops( c, v );
    ops.exceptions_all();

    //
    try
    {
        //
        if ( ops >> OptionPresent( '?', "help" ) )
        {
            usage();
            return false;
        }

        //
        if ( ! ( ops >> Option( 'l', "ldif",  ldpfile_ ) ) )
        {
            cerr << "Missiing tnsnames.ora file [-l|--ldif]\n";
            usage();
            return false;
        }

        //
        vector<string> opt;
        ops >> GlobalOption( opt );

        //
        if ( ! ( opt.size() == 1 ) )
        {
            cerr << "Missing TNS Names output file [<file>|-]\n";
            usage();
            return false;
        }
        else
            tnsfile_ = opt.at( 0 );

        //
        ops.end_of_options();
    }
    catch ( TooManyOptionsEx& ex )
    {
        cerr << "Too many options specified!\n";
        usage();
        return false;
    }
    catch ( GetOptEx& ex )
    {
        cerr << "Invalid or missing option!\n";
        usage();
        return false;
    }

    //
    return true;    
}
