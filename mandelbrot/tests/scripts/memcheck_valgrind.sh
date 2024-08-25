#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: ./memcheck_valgrind <executable_path>"
    exit 1
fi

EXECUTABLE_PATH=$1

valgrind --tool=memcheck --leak-check=full --log-file=valgrind_memcheck.log ${EXECUTABLE_PATH}

tail -n 20 valgrind_memcheck.log
