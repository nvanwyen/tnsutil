//
// tns2ldif.cpp
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
#include <fstream>
#include <vector>
#include <algorithm>

//
#include "tns2ldif.h"
#include "opt.h"
#include "exp.h"

//
#define LDAP_MOD        "changetype: add"


//
using namespace std;
using namespace mti;

//
void usage()
{
//           0--------1---------2---------3---------4---------5---------6---------7---------8
    cout << "Usage: " << UTIL_APP << " options <tnsnames.ldif>|<->\n";
    cout << "where:\n";
    cout << "   tnsnames.ldif         : output LDIF file name\n";
    cout << "   <->                   : output LDIF to stdout\n";
    cout << "options:\n";
    cout << "    -t | --tns     <val> : tnsnames.ora file location\n";
    cout << "   [-b | --base]   <val> : Base DN for LDIF output\n";
    cout << "   [-f | --format] <val> : LDIF output type, add|mod\n";
    cout << "   [-n | --nosort]       : Do not sort entries, before outputting results\n";
    cout << "   [-? | --help]         : Show the utility help and usage\n";
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
    app app( argc, argv );

    //
    if ( app.ok() )
        rc = app.run();
    else
        rc = 1;

    //
    return rc;
}

//
app::app( int c, char** v ) : tns_( new mti::tns() ),
                              ldpfile_( "" ),
                              tnsfile_( "" ),
                              format_( "" ),
                              sort_( true ),
                              ok_( false )
{
    ok_ = options( c, v );
}

//
int app::run()
{
    int rc = 1;

    using namespace std;

    //
    if ( tns_->load_tnsnames() > 0 )
    {
        bool       opn = false;
        streambuf* buf;
        ofstream   fil;

        //
        tns::entries ent = tns_->tns_entries();

        //
        if ( sort_ )
            tns_->sort_entries( ent );

        //
        if ( ldpfile_ != "-" )
        {
            fil.open( ldpfile_.c_str() );

            if ( fil.is_open() )
            {
                buf = fil.rdbuf();
                opn = true;
            }
            else
                buf = cout.rdbuf();
        }
        else
        {
            buf = cout.rdbuf();
            opn = true;
        }

        //
        std::ostream ldif( buf );

        //
        if ( opn )
        {
            // header
            ldif << "# Source:  " << tnsfile_ << endl;
            ldif << "# Created: " << now() << endl;
            ldif << endl;

            // data
            for ( tns::item i = ent.begin(); i != ent.end(); ++i )
            {
                // dn
                ldif << "# " << (*i).name << endl;
                ldif << "dn: cn=" << (*i).name << "," << base_ << endl;

                // add|mod format
                if ( format_ == "mod" )
                    ldif << LDAP_MOD << endl;

                // objectclasses
                ldif << "objectclass: top" << endl;
                ldif << "objectclass: orclService" << endl;
                ldif << "objectclass: orclContainer" << endl;
                ldif << "objectclass: orclNetService" << endl;

                // attributes
                ldif << "cn: " << (*i).name << endl;
                ldif << "orclNetDescString: " << (*i).desc << endl;

                //
                ldif << endl;
            }

            //
            cout << "Processed " << ent.size() << " item(s)\n";
        }
        else
        {
            cerr << "Could not open LDIF file [" << ldpfile_ << "]" << endl;
            rc = 1;
        }
    }
    else
        cerr << "No TNS entries found!\n";

    return rc;
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
        if ( ops >> OptionPresent( 't', "tns" ) )
        {
            //
            ops >> Option( 't', "tns",  tnsfile_ );

            //
            try
            {
                tns_->tnsnames( tnsfile_ );
            }
            catch ( exp& x )
            {
                cerr << x.what() << "\n";
                return false;
            }
        }
        else
            tnsfile_ = tns_->tnsnames();

        //
        if ( ops >> OptionPresent( 'b', "base" ) )
        {
            //
            ops >> Option( 'b', "base",  base_ );
        }
        else
            base_ = "cn=OracleContext";

        //
        if ( ops >> OptionPresent( 'f', "format" ) )
        {
            //
            ops >> Option( 'f', "format",  format_ );
        }
        else
            format_ = "add";

        //
        vector<string> fmt;
        fmt.push_back( "add" );
        fmt.push_back( "mod" );

        //
        if ( std::find( fmt.begin(), fmt.end(), format_ ) == fmt.end() )
            cerr << "--format must be \"add\" or \"mod\"!\n";
        
        //
        if ( ops >> OptionPresent( 'n', "nosort" ) )
            sort_ = false;
        else
            sort_ = true;

        //
        vector<string> opt;
        ops >> GlobalOption( opt );

        //
        if ( ! ( opt.size() == 1 ) )
        {
            cerr << "Missing LDIF output [<file>|-]\n";
            usage();
            return false;
        }
        else
            ldpfile_ = opt.at( 0 );

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

