#!/bin/bash

#
proj="/projects/test/cmake/audsys"
args=$@

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

cd $proj
chkrc $?

if [ "$args" = "" ] ; then

    if [ -f ${proj}/bin/audagent ] ; then
   
        color_echo red "Removing previous bin/audagent" 
        rm -f ${proj}/bin/audagent
    
    fi
    
    
    if [ -f ${proj}/bin/audfs ] ; then
   
        color_echo red "Removing previous bin/audfs" 
        rm -f ${proj}/bin/audfs
    
    fi
    
    if [ -f ${proj}/lib/libaudshield.so ] ; then
   
        color_echo red "Removing previous lib/libaudshield.so" 
        rm -f ${proj}/lib/libaudshield.so
    
    fi

else

    if [ "${args}" = "clean" ] ; then

        color_echo red "Cleaning project"
        rm -f ${proj}/bin/audagent        2>/dev/null
        rm -f ${proj}/bin/audfs           2>/dev/null
        rm -f ${proj}/lib/libaudshield.so 2>/dev/null

        rm -Rf ${proj}/build/*            2>/dev/null

        color_echo green "Clean complete"
        exit 0

    fi

fi

if [ -d ${proj}/build ] ; then

    color_echo green "Changing to build directory"
    cd ${proj}/build
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
    mkdir -p ${proj}/build
    chkrc $?

    color_echo green "Changing to build directory"
    cd ${proj}/build
    chkrc $?

fi

color_echo green "Executing CMake targets"
cmake ..
chkrc $?

cpu_count
color_echo green "Making Audit Shield"
make -j${cpu} ${opt}
chkrc $?


if [ -f ${proj}/bin/audagent ] ; then

    color_echo magenta "Found: bin/audagent" 

else

    color_echo red "Not found: bin/audagent"

fi  


if [ -f ${proj}/bin/audfs ] ; then

    color_echo magenta "Found: bin/audfs" 

else

    color_echo red "Not found: bin/audfs"

fi  

if [ -f ${proj}/lib/libaudshield.so ] ; then

    color_echo magenta "Found: lib/libaudshield.so" 

else

    color_echo red "Not found: lib/libaudshield.so"

fi  

