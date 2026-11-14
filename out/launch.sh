#!/bin/bash
echo "START"
cd "$(dirname "$(realpath "$0")")" || exit 1
export LD_LIBRARY_PATH=".:$LD_LIBRARY_PATH"
./MilitaryCrossing