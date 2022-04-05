#!/bin/bash
set -e
echo "CBR cpack RELEASE"

# From:
# https://stackoverflow.com/questions/59895/how-can-i-get-the-source-directory-of-a-bash-script-from-within-the-script-itsel
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="${SCRIPT_DIR}/.."
BUILD_DIR="${REPO_DIR}/build-release"

# Run our install script
pushd ${SCRIPT_DIR}
./cbr-install-release.sh
popd

pushd ${BUILD_DIR}
cpack -G ZIP -C Release

# Only on Mac
# cpack -C Release
popd
