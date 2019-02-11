#!/bin/bash

ROOT=`pwd`

BUILD_DIR="$ROOT/build"
INC_DIR="$ROOT/include"

function build()
{
  echo build start [$ROOT]
  rm -rf $BUILD_DIR
  mkdir $BUILD_DIR && cd $BUILD_DIR
  cmake .. -DBUILD_TYPE=coverage
  make
  echo build finish
}

function run-tests()
{
  find "$1" -name "test_*.out" -print0 | while read -d $'\0' f
  do
    "$f"
  done
}

build

run-tests $BUILD_DIR

