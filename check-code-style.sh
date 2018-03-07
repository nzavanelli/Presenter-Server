#!/bin/bash -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo "Check for consistent line endings"
! grep -IUlr $'\r' ${SCRIPT_DIR}/src

echo "Check for tab characters"
! grep -IUPr "\t" ${SCRIPT_DIR}/src
