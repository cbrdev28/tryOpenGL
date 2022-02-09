#!/bin/bash
set -e
echo "CBR clang-format"

# From:
# https://stackoverflow.com/questions/59895/how-can-i-get-the-source-directory-of-a-bash-script-from-within-the-script-itsel
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="${SCRIPT_DIR}/.."

pushd ${REPO_DIR}
# This command may need to be refactored when adding more files
clang-format --style=file --verbose -i ./*.cc ./src/*.h
popd
