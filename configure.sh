#!/bin/bash

#
args=$@
proj=`dirname $0`

#
function color_echo()
{
    local color=$1;
    local exp=$2;

    if ! [[ $color =~ '^[0-9]$' ]] ; then

        case $(echo $color | tr '[:upper:]' '[:lower:]') in
              black) color=0 ;;
                red) color=1 ;;
              green) color=2 ;;
             yellow) color=3 ;;
               blue) color=4 ;;
            magenta) color=5 ;;
               cyan) color=6 ;;
            white|*) color=7 ;; # white or "other"
        esac

    fi

    tput setaf $color;
    echo "** $exp";
    tput sgr0;
}

#
function cpu_count()
{
    cpu=`grep -c ^processor /proc/cpuinfo`

    if [[ -z "${cpu}" ]] ; then

        cpu=0

    fi

    cpu=$((cpu + 1))

    color_echo cyan "Using ${cpu} CPU jobs for make"
}

#
function chkrc()
{
    local rc=$1

    if [ $rc -ne 0 ] ; then

        color_echo red "Failed [$rc]!"
        exit $rc

    fi
}

#
function chkproj()
{
    if [ -f ${dir}/VERSION ] ; then

        prj=`grep "name: "    ${dir}/VERSION | awk -F ":" '{print $2}' | tr -d ' '`
        ver=`grep "version: " ${dir}/VERSION | awk -F ":" '{print $2}' | tr -d ' '`

        if [ "${prj}" != "TNS*Util" ] ; then

            color_echo red "Cannot confirm project version name \"TNS*Util\""
            exit 1

        fi

        if [[ -z "${ver}" ]] ; then

            color_echo red "Cannot confirm project build version"
            exit 1

        fi

        color_echo red "Auto-configuration: ${prj} - ${ver}"

    else

        color_echo red "This does not appear to be a TNS-Util project!"
        exit 1

    fi
}

#
cd $proj
chkrc $?

dir=`pwd`

#
chkproj

#
if [ "$args" = "" ] ; then

    if [ -f ${dir}/bin/tns2ldif ] ; then
   
        color_echo red "Removing previous bin/tns2ldif" 
        rm -f ${dir}/bin/tns2ldif
    
    fi
    
    
    if [ -f ${dir}/bin/ldif2tns ] ; then
   
        color_echo red "Removing previous bin/ldif2tns" 
        rm -f ${dir}/bin/ldif2tns
    
    fi
    

    if [ -f ${dir}/bin/tns2ldap ] ; then
   
        color_echo red "Removing previous bin/tns2ldap" 
        rm -f ${dir}/bin/tns2ldif
    
    fi
    
    
    if [ -f ${dir}/bin/ldap2tns ] ; then
   
        color_echo red "Removing previous bin/ldap2tns" 
        rm -f ${dir}/bin/ldif2tns
    
    fi

else

    if [ "${args}" = "clean" ] ; then

        color_echo red "Cleaning project"
        rm -f ${dir}/bin/tns2ldif        2>/dev/null
        rm -f ${dir}/bin/ldif2tns        2>/dev/null
        rm -f ${dir}/bin/tns2ldap        2>/dev/null
        rm -f ${dir}/bin/ldap2tns        2>/dev/null

        rm -Rf ${dir}/build/*            2>/dev/null

        color_echo green "Clean complete"
        exit 0

    fi

fi

#
if [ -d ${dir}/build ] ; then

    color_echo green "Changing to build directory"
    cd ${dir}/build
    chkrc $?

    if [ -f Makefile ] ; then
   
        color_echo cyan "Cleaning build project" 
        make clean
        chkrc $?
    
    fi
   
    color_echo red "Removing previous build files"
    rm -Rf *
    chkrc $?

else

    color_echo green "Creating build directory"
    mkdir -p ${dir}/build
    chkrc $?

    color_echo green "Changing to build directory"
    cd ${dir}/build
    chkrc $?

fi

color_echo green "Executing CMake targets"
cmake ..
chkrc $?

cpu_count
color_echo green "Making TNS utilities"
make -j${cpu} ${opt}
chkrc $?


if [ -f ${dir}/bin/tns2ldif ] ; then

    color_echo magenta "Found: bin/tns2ldif" 

else

    color_echo red "Not found: bin/tns2ldif"

fi

if [ -f ${dir}/bin/ldif2tns ] ; then

    color_echo magenta "Found: bin/ldif2tns" 

else

    color_echo red "Not found: bin/ldif2tns"

fi


if [ -f ${dir}/bin/tns2ldap ] ; then

    color_echo magenta "Found: bin/tns2ldap" 

else

    color_echo red "Not found: bin/tns2ldap"

fi

if [ -f ${dir}/bin/ldap2tns ] ; then

    color_echo magenta "Found: bin/ldap2tns" 

else

    color_echo red "Not found: bin/ldap2tns"

fi

