#!/bin/bash

if [ ! -e Compiler ]; then
    echo "test-error: Compiler does not exist."
    exit 1
fi

./Compiler

if [ $? -ne 0 ]; then
    echo "test-error: failed to compile."
    exit 1
fi
