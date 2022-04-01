#!/bin/bash
set -e
echo "CBR clang-format"

# From:
# https://stackoverflow.com/questions/59895/how-can-i-get-the-source-directory-of-a-bash-script-from-within-the-script-itsel
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="${SCRIPT_DIR}/.."

pushd ${REPO_DIR}
# This command may need to be refactored when adding more files
# And use a `find` to list files
clang-format --style=file --verbose -i\
    ./*.cc ./src/MainAppController/*.cc ./src/MainAppController/*.h\
    ./src/MainAppController/common/*.cc ./src/MainAppController/common/*.h\
    ./src/MainAppController/helpers/*.cc ./src/MainAppController/helpers/*.h\
    ./src/MainAppController/listeners/*.h\
    ./src/MainAppController/managers/*.cc ./src/MainAppController/managers/*.h\
    ./src/MainAppController/models/*.h\
    ./src/MainAppController/tests/*.cc ./src/MainAppController/tests/*.h
popd
