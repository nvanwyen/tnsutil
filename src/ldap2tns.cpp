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
#include <fstream>
#include <vector>
#include <algorithm>

//
#include "ldap2tns.h"
#include "opt.h"

#include "uri.h"

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
    cout << "   <->                   : TNS output to stdout\n";
    cout << "options:\n";
    cout << "   [-D | --DN]     <val> : Distinguished name of the LDAP account\n";
    cout << "   [-w | --passwd| <val> : Distinguished name password\n";
    cout << "    -q | --prompt]       : Prompt for the Distinguished name password\n";
    cout << "   [-u | --url|    <val> : Use the LDAP Url string as ldap[s]://<host>:<port>\n";
    cout << "    -h | --host &  <val> : LDAP server host or IP\n";
    cout << "    -p | --port]   <val> : LDAP server port number\n";
    cout << "   [-b | --base]   <val> : Base (parent) entry where entries are managed\n";
    cout << "   [-a | --admin|  <val> : Location of the TNS_ADMIN directory\n";
    cout << "    -l | --ldap &  <val> : The ldap.ora file\n";
    cout << "    -s | --sqlnet] <val> : The sqlnet.ora file\n";
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
                              tnsfile_( "" ),
                              dn_( "" ),
                              pw_( "" ),
                              url_( "" ),
                              host_( "" ),
                              port_( 0 ),
                              root_( "" ),
                              admin_( "" ),
                              ldap_( "" ),
                              sqlnet_( "" ),
                              sort_( true ),
                              ok_( false )
{
    ok_ = options( c, v );
}

//
int app::run()
{
    using namespace std;

    int rc = 0;
    tns::store sto;

    //
    if ( url_.length() > 0 )
    {
        //
        sto.url( url_ );
    }
    else
    {
        //
        if ( ( host_.length() > 0 ) && ( port_ > 0 ) )
        {
            //
            sto.host = host_;
            sto.port = port_;
        }
        else
        {
            //
            if ( ldap_.length() > 0 )
                tns_->ldap( ldap_ );
            else
                tns_->ldap( tns_->ldap() );

            //
            if ( sqlnet_.length() > 0 )
                tns_->sqlnet( sqlnet_ );
            else
                tns_->sqlnet( tns_->sqlnet() );

            //
            sto = tns_->resolve_directory();
        }
    }

    //
    if ( ! sto.ok() )
    {
        cerr << "The LDAP server information is missing or invalid!\n";
        rc = 1;
    }
    else
    {
        //
        if ( dn_.length() > 0 )
            sto.dn = dn_;

        //
        if ( pw_.length() > 0 )
            sto.pw = pw_;

        //
        tns_->ldapstore( sto );

        //
        if ( tns_->load_ldap() > 0 )
        {
            bool       opn = false;
            streambuf* buf;
            ofstream   fil;

            //
            if ( tnsfile_ != "-" )
            {
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
                tnsfile << "# Source:  " << sto.url() << endl;
                tnsfile << "# Created: " << now() << endl;
                tnsfile << endl;

                //
                for ( tns::item i = ent.begin(); i != ent.end(); ++i )
                {
                    tnsfile << "# " << (*i).name << endl;
                    tnsfile << tns_->pretty( *i ) << endl;
                }

                //
                tnsfile << endl;

                //
                cout << "Processed " << ent.size() << " item(s)\n";
            }
        }
        else
            cerr << "No LDAP entries found!\n";
    }

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
        if ( ops >> OptionPresent( 'D', "DN" ) )
        {
            //
            ops >> Option( 'D', "DN",  dn_ );
        }
        else
            dn_ = "";

        //
        if ( ( ops >> OptionPresent( 'q', "prompt" ) )
          && ( ops >> OptionPresent( 'w', "passwd" ) ) )
        {
            cerr << "Cannot specify both --prompt [-q] and --passwd [-w] together\n";
            return false;
        }

        //
        if ( ops >> OptionPresent( 'q', "prompt" ) )
        {
            //
            pw_ = password();
        }
        else
        {
            //
            if ( ops >> OptionPresent( 'w', "passwd" ) )
            {
                //
                ops >> Option( 'w', "passwd",  pw_ );
            }
            else
            {
                pw_ = "";
            }
        }

        //
        if ( ( ( dn_.length() >  0 ) && ( pw_.length() == 0 ) )
          || ( ( dn_.length() == 0 ) && ( pw_.length() >  0 ) ) )
        {
            cerr << "--DN [-D] and password [--passwd [-w] | --prompt [-q]] must be supplied together\n";
            return false;
        }

        //
        if ( ops >> OptionPresent( 'u', "url" ) )
        {
            //
            ops >> Option( 'u', "url",  url_ );
        }
        else
            url_ = "";

        //
        if ( ops >> OptionPresent( 'h', "host" ) )
        {
            //
            ops >> Option( 'h', "host",  host_ );
        }
        else
            host_ = "";

        //
        if ( ops >> OptionPresent( 'p', "port" ) )
        {
            //
            ops >> Option( 'p', "port",  port_ );
        }
        else
            port_ = 0;

        //
        if ( ops >> OptionPresent( 'b', "base" ) )
        {
            //
            ops >> Option( 'b', "base",  root_ );
        }
        else
            root_ = "";

        //
        if ( ops >> OptionPresent( 'a', "admin" ) )
        {
            //
            ops >> Option( 'a', "admin",  admin_ );
        }
        else
            admin_ = "";

        //
        if ( ops >> OptionPresent( 'l', "ldap" ) )
        {
            //
            ops >> Option( 'l', "ldap",  ldap_ );
        }
        else
            ldap_ = "";

        //
        if ( ops >> OptionPresent( 's', "sqlnet" ) )
        {
            //
            ops >> Option( 's', "sqlnet",  sqlnet_ );
        }
        else
            sqlnet_ = "";

        //
        if ( ( admin_.length() > 0 ) && ( ( ldap_.length() > 0 ) || ( sqlnet_.length() > 0 ) ) )
        {
            cerr << "Cannot use --admin [-a] together with --ldap [-l] or --sqlnet [-s]\n";
            return false;
        }
        else
        {
            if ( ( ldap_.length() > 0 ) || ( sqlnet_.length() > 0 ) )
            {
                if ( ! ( ( ldap_.length() > 0 ) && ( sqlnet_.length() > 0 ) ) )
                {
                    cerr << "Options --ldap [-l] and --sqlnet [-s] must both be specified together\n";
                    return false;
                }
            }
        }

        //
        if ( ( url_.length() > 0 ) && ( ( host_.length() > 0 ) || ( port_ > 0 ) ) )
        {
            cerr << "Cannot use --url [-u] together with --host [-h] or --port [-p]\n";
            return false;
        }
        else
        {
            if ( ( host_.length() > 0 ) || ( port_ > 0 ) )
            {
                if ( ! ( ( host_.length() > 0 ) && ( port_ > 0 ) ) )
                {
                    cerr << "Options --host [-h] and --port [-p] must both be specified together\n";
                    return false;
                }
            }
        }

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
