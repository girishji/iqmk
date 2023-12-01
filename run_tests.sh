#!/bin/bash
rm -rf build
mkdir -p build \
    && cd build \
    && cmake -DQMK_BUILD_TYPE:STRING=Test ..

if [[ $? -eq 0 ]]; then
    if [[ $(uname -s) == "Darwin" ]]; then
        cmake --build . -j 4
    else
        cmake --build .
    fi
fi

[[ $? -eq 0 ]] && ctest
