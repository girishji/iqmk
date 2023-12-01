#!/bin/bash
mkdir -p build \
    && cd build \
    && cmake -DQMK_BUILD_TYPE:STRING=Test .. \
    && cmake --build . \
    && ctest
