#!/bin/bash
set -e
echo "CBR ctest"

# From:
# https://stackoverflow.com/questions/59895/how-can-i-get-the-source-directory-of-a-bash-script-from-within-the-script-itsel
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="${SCRIPT_DIR}/.."
BUILD_DIR="${REPO_DIR}/build"

# Run our format & build scripts
pushd ${SCRIPT_DIR}
./cbr-build.sh
popd

pushd ${BUILD_DIR}
ctest -VV
popd
