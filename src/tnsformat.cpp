//
// tnsformat.cpp
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
#include <fstream>
#include <iostream>
#include "tnsformat.h"
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
    cout << "   <tnsnames.ora>     : TNS Names input file\n";
    cout << "options:\n";
    cout << "   [-o | --stdout]    : Write only to standard output\n";
    cout << "   [-n | --nosort]    : Do not sort entries results\n";
    cout << "   [-k | --nobackup]  : Do not backup file\n";
    cout << "   [-? | --help]      : Show the utility help and usage\n";
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
                              sort_( true ),
                              backup_( true ),
                              ok_( false )
{
    ok_ = options( c, v );
}

//
int app::run()
{
    int rc = 1;

    //
    using namespace std;

    //
    if ( tnsfile_.length() > 0 )
    {
        //
        if ( tns_->load_tnsnames( tnsfile_ ) > 0 )
        {
            bool opn = false;
            streambuf* buf;
            ofstream   fil;

            //
            if ( ( backup_ ) && ( ! stdout_ ) )
                bak bak( tnsfile_ );

            //
            tns::entries ent = tns_->tns_entries();

            //
            if ( sort_ )
                tns_->sort_entries( ent );

            //
            if ( stdout_ )
            {
                buf = cout.rdbuf();
                opn = true;
            }
            else
            {
                //
                fil.open( tnsfile_.c_str(), ofstream::out | ofstream::binary | ofstream::trunc );

                buf = fil.rdbuf();
                opn = true;
            }

            //
            std::ostream out( buf );

            if ( opn )
            {
                out << "# Source:  " << tnsfile_ << endl;
                out << "# Created: " << now() << endl;
                out << endl;

                // entries
                for ( tns::item i = ent.begin(); i != ent.end(); ++i )
                {
                    out << "#" << endl;
                    out << tns_->pretty( *i ) << endl;
                    out << endl;
                }

                //  
                cout << "# Processed: " << ent.size() << " item(s)\n";
            }
            else
                cerr << "Could not open output file [" << tnsfile_ << "] for writing!" << endl;
        }
        else
            cerr << "No TNS entries found!\n";
    }
    else
        cerr << "No TNS file spcified!\n";

    //
    return rc;
}

//
bool app::options( int c, char** v )
{
    using namespace std;

    //
    opt ops( c, v );
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
        if ( ops >> OptionPresent( 'o', "stdout" ) )
            stdout_ = true;
        else
            stdout_ = false;

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
            cerr << "Missing TNS Names file [<file>|-]\n";
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
    catch ( optex& ex )
    {
        cerr << "Invalid or missing option! " << ex.what() << endl;
        usage();
        return false;
    }

    //
    return true;    
}
