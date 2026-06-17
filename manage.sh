#!/usr/bin/env bash

case $1 in
  clean)
    rm -rf build  ;;
  build)
    cmake -S . -B build
    cmake --build build ;;
  run)
    shift
    ./build/main "$@" ;;
  *)
    echo "Unsupported operation."
    echo
esac