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

// boost

// local
#include "env.h"
#include "tns.h"
#include "ora.h"
#include "exp.h"

using namespace std;
using namespace mti;

//
void usage()
{
    //      0.........1.........2.........3.........4.........5.........6.........7.........8
    cout << "Usage TNS [COUNT]" << endl << endl;

    cout << "  where TNS is an entry or fully qualifed description and COUNT is the number of" << endl
         << "  ping its (default 1) or 0 (zero) for unlimited"   << endl << endl;

    cout << "  examples: $ oraping orcl"                                << endl << endl;

    cout << "            $ oraping //server.com:5521/orcl 3"            << endl << endl;

    cout << "            $ oraping \"(description = \\"                         << endl;
    cout << "                           (address = \\"                          << endl;
    cout << "                               (protocol = tcp) \\"                << endl;
    cout << "                               (host = server.com) \\"             << endl;
    cout << "                               (port = 1521)) \\"                  << endl;
    cout << "                           (connect_data = \\"                     << endl;
    cout << "                               (server = dedicated) \\"            << endl;
    cout << "                               (service_name = orcl)))\""  << endl << endl;
}

//
bool is_numeric( char *val )
{
    bool ok = true;

    //
    int sz = strlen( val );
    int it = 0;

    //
    while ( it < sz )
    {
        //
        if ( !isdigit( val[ it ] ) )
        {
            //
            ok = false;
            break;
        }

        //
        it++;

    }

    //
    return ok;
}

//
string env( string var )
{
	string val;

#if !defined(DOS) && !defined(_WIN32) && !defined(WINSOCK)
    char* tmp = ::getenv( var.c_str() );
	val = string( ( ( tmp == NULL ) ? "" : tmp ) );
#else
	char* tmp = NULL;
	size_t sz;

	::getenv_s( &sz, NULL, 0, var.c_str() );

	if ( sz > 0 )
	{
		tmp = (char*)::malloc( sz * sizeof(char) );

		if ( tmp )
		{
			::getenv_s( &sz, tmp, sz, var.c_str() );
			val = string( tmp );

			::free( tmp );
		}
	}
#endif

	return val;
}

//
string sid( void )
{
    string val;

    //
    val = env( "TWO_TASK" );

    //    
    if ( val.length() == 0 )
    {
        //
        val = env( "ORACLE_SID" );

        //
        if ( val.length() > 0 )
            cout << "using environment variable ORACLE_SID for TNS\n" << endl;
    }
    else
        cout << "using environment variable TWO_TASK for TNS\n" << endl;

    //
    return val;
}
