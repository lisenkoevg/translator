#!/bin/bash

main() {
  if [ "$1" = "all" ]; then
    test_all
    exit
  fi
  if [ -z "$1" ]; then
    usage
    exit
  fi
  p=`basename $1`
  pexe=$p.exe
  pf=`pwd`/$pexe
  if [ ! -e $pexe ]; then
    echo Program "$pexe" not exists.
    exit
  fi
  testdir=test_$p
  if [ ! -d $testdir ]; then
    echo Test dir "$testdir" not exists.
    exit
  fi
  mask='*'
  if [ "$2" != "" ]; then
    mask=$2
  fi
  param1=
  if [ "$3" != "" ]; then
    param1=$3
  fi
  c=0
  cp=0
  cf=0
  pushd $testdir > /dev/null
  set +f
  if ! stat $mask.in > /dev/null 2>&1 ; then
    echo "Tests $mask.in not exist"
    exit 0
  fi
  for t in $mask.in; do
    run_test $t
  done
  popd > /dev/null
  if [[ $c == $cp ]]; then
    beep 5000 100
    echo_green Total: $c, passed: $cp, failed: $cf
    return 0
  else
    beep 1000 100
    echo_red Total: $c, passed: $cp, failed: $cf
    return 1
  fi
}

run_test() {
  tn=`basename $1 .in`
  ta=${tn}_actual.out
  $pf $param1<$1| tr -d '\r' > $ta
  if diff -N $tn.out $ta > /dev/null; then
    echo $1... ok
    rm $ta
    (( cp++ ))
  else
    echo $1... FAILED
    (( cf++ ))
  fi
  (( c++ ))
}

beep() { uname | grep CYGWIN > /dev/null && nircmd beep $1 $2; }

test_all() {
  set +f
  for dir in $(ls -d [0-9][0-9] | sort -V); do
    set -f
    pushd $dir > /dev/null
    echo ==================== Testing $(basename $(pwd)) ========================
    make -B -j --output-sync=target
    if [[ $? -ne 0 ]] ; then
      echo_red "make: error in directory $dir"
    else
     make test || echo_red "make test: error in directory $dir"
     make clean
    fi
    popd > /dev/null
  done
}

usage() {
  echo "Usage: $0 [program] [filemask|all] [param1]"
}

echo_green() {
  echo -e '\E[37;32m'"\033[1m${*}\033[0m"
}

echo_red() {
  echo -e '\E[37;31m'"\033[1m${*}\033[0m"
}

set -f
main $*
