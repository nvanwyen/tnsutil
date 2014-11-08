//
// ldif2tns.cpp
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
#include "ldif2tns.h"
#include "bak.h"
#include "opt.h"

//
using namespace std;
using namespace mti;

//
void usage()
{
//           0--------1---------2---------3---------4---------5---------6---------7---------8
    cout << "Usage: " << UTIL_APP << " options <tnsnames.ora>|<->\n";
    cout << "where:\n";
    cout << "   <tnsnames.ora>        : TNS Names output file\n";
    cout << "   <->                   : output to stdout\n";
    cout << "options:\n";
    cout << "    -l | --ldif    <val> : The LDIF file location\n";
    cout << "   [-n | --nosort]       : Do not sort entries, before outputting results\n";
    cout << "   [-k | --nobackup]     : Do not backup output file name if it already exists\n";
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
                              tnsfile_( "" ),
                              ldpfile_( "" ),
                              sort_( true ),
                              backup_( true ),
                              ok_( false )
{
    ok_ = options( c, v );
}

//
int app::run()
{
    using namespace std;

    int rc = 1;

    //
    fstream ldif( ldpfile_.c_str() );

    //
    if ( ldif.is_open() )
    {
        //
        if ( tns_->load_ldif( ldif.rdbuf() ) > 0 )
        {
            //
            bool       opn = false;
            streambuf* buf;
            ofstream   fil;

            //
            if ( tnsfile_ != "-" )
            {
                //
                if ( backup_ )
                    bak bak( tnsfile_ );

                fil.open( tnsfile_.c_str() );
            
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
            std::ostream tnsfile( buf );

            //
            if ( opn )
            {
                //
                tns::entries ent = tns_->tns_entries();

                //
                if ( sort_ )
                    tns_->sort_entries( ent );

                // header
                tnsfile << "# Source:  " << ldpfile_ << endl;
                tnsfile << "# Created: " << now() << endl;
                tnsfile << endl;

                //
                for ( tns::item i = ent.begin(); i != ent.end(); ++i )
                {
                    tnsfile << "# " << (*i).name << endl;
                    tnsfile << tns_->pretty( *i ) << endl;
                    tnsfile << endl;
                }

                //
                cout << "Processed " << ent.size() << " item(s)\n";
            }
        }
        else
            cerr << "No entries found!\n";
    }
    else
        cerr << "Could not open LDIF file!\n";

    //
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
        if ( ! ( ops >> Option( 'l', "ldif",  ldpfile_ ) ) )
        {
            cerr << "Missiing tnsnames.ora file [-l|--ldif]\n";
            usage();
            return false;
        }

        //
        if ( ops >> OptionPresent( 'n', "nosort" ) )
            sort_ = false;
        else
            sort_ = true;

        //
        if ( ops >> OptionPresent( 'k', "nobackup" ) )
            backup_ = false;
        else
            backup_ = true;

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
        cerr << "Too many options specified! " << ex.what() << endl;
        usage();
        return false;
    }
    catch ( GetOptEx& ex )
    {
        cerr << "Invalid or missing option! " << ex.what() << endl;
        usage();
        return false;
    }

    //
    return true;    
}
