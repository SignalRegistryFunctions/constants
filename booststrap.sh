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
  cmake -B build -DBUILD_SHARED_LIBS=OFF -DARGTABLE3_ENABLE_TESTS=OFF -DARGTABLE3_ENABLE_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX="$LOCALAPPDATA"
  cmake --build build --config Release
  cmake --install build 
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

if [ "$1" == "install" ]; then

  echo ===========================================================================
  echo "[INFO] Installing ..."
  echo ===========================================================================
  rm -rf build
  cmake -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build

  if [[ "$OSTYPE" == "linux-gnu" ]]; then
    echo "Here"
  elif [[ "$OSTYPE" == "darwin" ]]; then
          # Mac OSX
    echo "Here"
  elif [[ "$OSTYPE" == "cygwin" ]]; then
          # POSIX compatibility layer and Linux environment emulation for Windows
    echo "Here"
  elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
    cmake.exe -B build -DCMAKE_INSTALL_PREFIX="$LOCALAPPDATA"/SignalRegistry  
          # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
  elif [[ "$OSTYPE" == "win32" ]]; then
          # I'm not sure this can happen.
    echo "Here"
  elif [[ "$OSTYPE" == "freebsd" ]]; then
          # ...
    echo "Here"
  else
          # Unknown.
    echo "Here"
  fi
  cmake --build build --config Release --target INSTALL

fi

if [ "$1" == "publish" ]; then

  echo ===========================================================================
  echo "[INFO] Pıblishing ..."
  echo ===========================================================================
  VERSION=`cat VERSION`
  RELEASE=`git tag -l | tail -1`
  if [ "$RELEASE" = "" ];then
    gh release create "v$VERSION" --generate-notes
  else 
    gh release create "v$VERSION" --generate-notes --notes-start-tag `git tag -l | tail -1`
  fi
  git fetch --tags origin
  # echo $VERSION
  # cd build
  # cpack --config CPackConfig.cmake 
  # cd ..
  # if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  #       # ...
  # elif [[ "$OSTYPE" == "darwin"* ]]; then
  #         # Mac OSX
  # elif [[ "$OSTYPE" == "cygwin" ]]; then
  #         # POSIX compatibility layer and Linux environment emulation for Windows
  # elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
  #         # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
  # elif [[ "$OSTYPE" == "win32" ]]; then
  #         # I'm not sure this can happen.
  # elif [[ "$OSTYPE" == "freebsd"* ]]; then
  #         # ...
  # else
  #         # Unknown.
  # fi

fi

if [ "$1" == "unpublish" ]; then

  echo ===========================================================================
  echo "[INFO] Remove Publishing ..."
  echo ===========================================================================
  RELEASE=`git tag -l | tail -1`
  gh release delete $RELEASE -y && git push --delete origin $RELEASE && git tag -d $RELEASE 

fi