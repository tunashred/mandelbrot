#!/bin/bash

if ["$#" -ne 3]; then
	echo "Usage: $0 <executable_path> <profile_file_path> <output_path>"
	exit 1
fi

EXECUTABLE_PATH=$1
PROFILE_FILE_PATH=$2
OUTPUT_FILE_PATH=$3

env CPUPROFILE=${PROFILE_FILE_PATH} ${EXECUTABLE_PATH}

pprof --text ${EXECUTABLE_PATH} ${PROFILE_FILE_PATH} > ${OUTPUT_FILE_PATH}

cat ${OUTPUT_FILE_PATH}
