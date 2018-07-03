#!/bin/bash -x



# Get absolute path to base dir
MYDIR=$(dirname "$0")
cd "$MYDIR"
BASEDIR="$PWD"

try() { $@ || fail "Command $* failed -- check above for details" ;}

fail() {
   set +x # Turn off command listing now, if it's on
   echo "FAILED!  Message follows:"
   echo $@
   echo "Halted, hit return to continue, or give up..."
   read x

}


check_expected() {
for f in $@ ; do
   [ -e $f ] || fail "Expected result file $f not present (not built)!"
done
}

check_os(){
    result=`lsb_release -i`
    os=`echo $result |awk -F":" '{print $2}' |tr A-Z a-z`
    if [[ $os =~ "ubuntu" ]] ; then
      sudo apt-get install libexpat1-dev cmake gcc g++ automake autoconf
    elif [[ $os =~ "centos" || $os =~ "redhat" || $os =~ "fedora" ]] ; then
      sudo yum install expat-devel cmake gcc gcc-c++ automake autoconf
    else
      echo 'Not Known OS. Exiting!'
      exit 1
    fi
}

install_prerequisites() {
  check_os
}


install_prerequisites


# Create application
cd "$BASEDIR" || fail
mkdir project
cd project/ || fail
cp  -r $BASEDIR/examples/* ./


cd "$BASEDIR/project" || fail
mkdir  -p build/generated
check_expected build  inc  src  xml CMakeLists.txt


cd build || fail
try cmake ..
try make 
check_expected SrvExample CltExample
chmod +x SrvExample CltExample

# Your output should look similiar. In the build direcory there should be two executables now: HelloWorldClient and HelloWorldService.

echo "You may now run $PWD/SrvExample &"
echo "and $PWD/CltExample"
# ./HelloWorldService &
# ./HelloWorldClient

