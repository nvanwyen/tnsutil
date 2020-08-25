//
// tns.h
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

#ifndef __TNS_H
#define __TNS_H

// c

// c++
#include <string>
#include <vector>

// boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

// local
#include "exp.h"
#include "uri.h"

//
typedef struct ldap LDAP;

//
using namespace std;

//
#define UNKNOWN_ORA     0
#define TNSNAMES_ORA    1
#define LDAP_ORA        2
#define SQLNET_ORA      4

//
namespace mti {

//
class tns
{
    public:
        //
        class entry
        {
            public:
                //
                enum origin { UNKNOWN, TNSNAMES, LDAP, EZCONNECT };

                //
                string name;
                string desc;
                origin type;

                entry() : name( "" ), desc( "" ), type( UNKNOWN )
                {}

                entry( string n, string d, int t = UNKNOWN )
                {
                    name = n;
                    desc = d;
                    type = (origin) t;
                }

                //
                string source()
                {
                    string t;

                    switch ( type )
                    {
                        case TNSNAMES:
                            t = "TNSNAMES";
                            break;

                        case LDAP:
                            t = "LDAP";
                            break;

                        case EZCONNECT:
                            t = "EZCONNECT";
                            break;

                        default:
                            t = "UNKNOWN";
                    }

                    return t;
                }

                //
                bool operator<( const entry& ent ) const
                {
                    return name < ent.name;
                }

                //
                bool operator>( const entry& ent ) const
                {
                    return ( ! operator<( ent ) );
                }

            protected:
            private:
        };

        //
        typedef vector<entry>   entries;
        typedef vector<string>  methods;

        typedef vector<entry>::iterator  item;
        typedef vector<string>::iterator method;

        //
        class store
        {
            //
            public:
                //
                string host;
                int    port;
                int    pssl;
                string root;
                //
                bool   secure;
                //
                string dn;
                string pw;
        
                //
                store()
                    : host( "" ), port( 0 ), pssl( 0 ), secure( false ), root( "" ) {}
        
                store( string h, int p )
                    : host( h ),  port( p ), pssl( 0 ), secure( false ), root( "" ) {}
        
                store( string h, int p, bool s )
                    : host( h ),  port( p ), pssl( 0 ), secure( s ),     root( "" ) {}
        
                store( string h, int p, bool s, string r )
                    : host( h ),  port( p ), pssl( 0 ), secure( s ),     root( r )  {}
        
                //
                store( string u ) { url( u ); }
        
                //
                ~store() {}
        
                //
                void url( string u )
                {
                    uri i( u ); 
        
                    //
                    host = i.host();
                    port = i.port();
        
                    //
                    if ( i.protocol().substr( i.protocol().length() - 1 ) == "s" )
                        secure = true;
                }
        
                //
                string url()
                {
                    string u;
        
                    //
                    if ( ! ok() )
                        throw exp( "Invalid ldap store object!", EXP_INVALID );
        
                    //
                    u = ( ( secure ) ? "ldaps" : "ldap" ) 
                           + string( "://" ) 
                           + host;
        
                    //
                    if ( ! ( ( secure && ( port == 636 ) ) || ( ! secure && ( port == 389 ) ) ) )
                    {
                        u += string( ":" ) 
                           + boost::lexical_cast<string>( port );
                    }
        
                    return u;
                }
        
                //
                bool ok()
                {
                    return ( ( host.length() > 0 ) && ( port > 0 ) );
                }

            protected:
            private:
        };
        
        //
        tns() {}
        ~tns() {}

        //
        entry& find( string ent );

        //
        string tnsadmin();
        string tnsadmin( string dir );

        //
        string sqlnet();
        string sqlnet( string filename );

        //
        string tnsnames();
        string tnsnames( string filename );

        //
        string ldap();
        string ldap( string filename );

        //
        store ldapstore();
        store ldapstore( store sto );

        //
        size_t load_methods();

        //
        size_t load_tnsnames();
        size_t load_ldap();

        //
        size_t load_ldif( streambuf* buf );

        //
        bool save_ldap( entry& ent, bool repl = true );

        //
        methods tns_methods() { return methods_; }
        entries tns_entries() { return entries_; }

        //
        void sort_entries( entries& ent );

        //
        string pretty( entry& ent );

        //
        store resolve_directory();

        //
        size_t load_tnsnames( std::string file );

    protected:
    private:
        //
        entry entry_;

        //
        string tnsadmin_;

        // files
        string sqlnet_;

        //
        string tnsnames_;
        string ldap_;

        // ldap store
        tns::store store_;

        // ldap.ora
        string root_;                   // root context

        // sqlnet.ora
        methods methods_;               // connection type (from sqlnet.ora)

        // tnsnames.ora and ldap://...
        entries entries_;               // list of all entries

        //
        string trim( string str );
        //
        string ltrim( string str );
        string rtrim( string str );

        //
        string lcase( string str );
        string ucase( string str );

        //
        string split( string str, int idx, string tok = "," );

        //
        bool mesg();
        bool ldap_exists( LDAP* ld, char* dn );
        bool ldap_delete( LDAP* ld, char* dn );

        //
        size_t add( string name, string desc, entry::origin type );
        size_t add( entry ent );

        // resolve a single entry
        entry resolve( string name, string desc );

        //
        string env( string var );       // get the value of a environment variable
        bool exists( string file );     // test if a file exists

        //
        string locate( string name );   // find a file based on the TNS search order


        //
        string format( string str );

        //
        string resolve( string ent );
        string compare( string dsc );
};

//
typedef boost::shared_ptr<tns> tnsnames;

}
#endif // __TNS_H
