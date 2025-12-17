#!/bin/bash


if [ "$1" == "dependencies" ]; then
  rm -rf dependencies
  mkdir dependencies

  echo ===========================================================================
  echo "[INFO] Function Dependencies ..."
  echo ===========================================================================

  echo ===========================================================================
  echo "[INFO] CLI Dependencies ..."
  echo ===========================================================================
  echo ---------------------------------------------------------------------------
  echo "Argtable3: A single-file, ANSI C, command-line parsing library that parses GNU-style command-line options"
  echo ---------------------------------------------------------------------------
  ARGTABLE_VERSION="v3.3.0.116da6c"
  curl -L "https://github.com/argtable/argtable3/releases/download/${ARGTABLE_VERSION}/argtable-${ARGTABLE_VERSION}.tar.gz" --output dependencies/argtable.tar.gz --silent 
  tar -xf dependencies/argtable.tar.gz --directory dependencies/
  rm dependencies/argtable.tar.gz
  mv "dependencies/argtable-${ARGTABLE_VERSION}/" dependencies/argtable/
  cd dependencies/argtable
  cmake -B build -DBUILD_SHARED_LIBS=OFF -DARGTABLE3_ENABLE_TESTS=OFF -DARGTABLE3_ENABLE_EXAMPLES=OFF
  cmake --build build --config Release
  cd ../..

  echo ---------------------------------------------------------------------------
  echo "Jansson: C library for encoding, decoding and manipulating JSON data"
  echo ---------------------------------------------------------------------------
  JANSSON_VERSION="2.14.1"
  curl -L https://github.com/akheron/jansson/releases/download/v${JANSSON_VERSION}/jansson-${JANSSON_VERSION}.tar.gz --output dependencies/jansson.tar.gz --silent
  tar -xf dependencies/jansson.tar.gz --directory dependencies/
  mv "dependencies/jansson-${JANSSON_VERSION}/" dependencies/jansson
  cd dependencies/jansson
  cmake -B build -DJANSSON_BUILD_DOCS=OFF -DJANSSON_BUILD_SHARED_LIBS=OFF -DJANSSON_WITHOUT_TESTS=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5
  cmake --build build --config Release
  cd ../..
fi 

if [ "$1" == "build" ]; then

  echo ===========================================================================
  echo "[INFO] Building ..."
  echo ===========================================================================
  rm -rf build
  cmake -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build

fi

if [ "$1" == "publish" ]; then

  echo ===========================================================================
  echo "[INFO] Pıblishing ..."
  echo ===========================================================================
  VERSION=`cat VERSION`
  echo $VERSION
  cd build
  cpack --config CPackConfig.cmake 
  cd ..

fi