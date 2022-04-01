#!/bin/bash
set -e
echo "CBR cmake configure & build"

# From:
# https://stackoverflow.com/questions/59895/how-can-i-get-the-source-directory-of-a-bash-script-from-within-the-script-itsel
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="${SCRIPT_DIR}/.."
BUILD_DIR="${REPO_DIR}/build"

pushd ${REPO_DIR}
mkdir -p ${BUILD_DIR}

pushd ${BUILD_DIR}
cmake ../
cmake --build .
popd

popd
