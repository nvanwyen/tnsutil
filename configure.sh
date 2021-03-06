#!/bin/bash

#
proj="TNS*Util"
tgts="bin/tns2ldif bin/ldif2tns bin/tns2ldap bin/ldap2tns bin/tnsformat"
dels="src/ver.h"

#
args=$@
root=`dirname $0`

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

        if [ "${prj}" != "${proj}" ] ; then

            color_echo red "Cannot confirm project version name \"${proj}\""
            exit 1

        fi

        if [[ -z "${ver}" ]] ; then

            color_echo red "Cannot confirm project build version"
            exit 1

        fi

        color_echo red "Auto-configuration: ${prj} - ${ver}"

    else

        color_echo red "This does not appear to be a ${proj} project!"
        exit 1

    fi
}

#
function clean()
{
    for t in ${tgts} ; do

        if [ -f ${dir}/${t} ] ; then

            color_echo red "Removing previous ${t}"
            rm -f ${dir}/${t} 2>/dev/null

        fi

    done
}

#
function validate()
{
    if [[ ! -z "${dels}" ]] ; then

        #
        for d in ${dels} ; do
    
            if [ -f ${dir}/${d} ] ; then
            
                color_echo magenta "Clean: ${d}" 
                rm -f ${dir}/${d} 2>/dev/null
    
            else
    
                color_echo red "Not found: ${d}"
            
            fi
    
        done

    fi

    if [[ ! -z "${tgts}" ]] ; then

        #
        for t in ${tgts} ; do
    
            if [ -f ${dir}/${t} ] ; then
            
                color_echo magenta "Found: ${t}" 
            
            else
            
                color_echo red "Not found: ${t}"
            
            fi
    
        done

    fi
}

#
cd $root
chkrc $?

dir=`pwd`

#
chkproj

#
if [ "$args" = "" ] ; then

    clean

else

    if [ "${args}" = "clean" ] ; then

        color_echo red "Cleaning project"

        clean
        rm -Rf ${dir}/build/* 2>/dev/null

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
cmake -Wno-dev ..
chkrc $?

cpu_count
color_echo green "Making TNS utilities"
make -j${cpu} ${opt}
chkrc $?

validate
