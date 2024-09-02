#!/bin/bash

generate_summary() {
    local file="$1"
    echo "File name: $file"
    echo "Function                             Bytes       Qualifiers"
    echo "-----------------------------------  ----------  -------------------"
    
    awk -F':' '{print $(NF)}' $file | awk -F' ' '{printf "%-35s %10s %20s\n", $1, $2, $3}'
    
    echo "============================================================"
    echo
}

for file in "$@"; do
    generate_summary "$file"
done
