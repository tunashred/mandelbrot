#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: ./memcheck_valgrind <executable_path> <log_save_path>"
    exit 1
fi

EXECUTABLE_PATH=$1
LOG_SAVE_PATH=$2

valgrind --tool=memcheck --leak-check=full --log-file="$LOG_SAVE_PATH/valgrind_memcheck.log" "$EXECUTABLE_PATH"

tail -n 20 "$LOG_SAVE_PATH/valgrind_memcheck.log"
