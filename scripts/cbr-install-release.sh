#!/bin/bash
set -e
echo "CBR cmake install RELEASE"

# From:
# https://stackoverflow.com/questions/59895/how-can-i-get-the-source-directory-of-a-bash-script-from-within-the-script-itsel
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR="${SCRIPT_DIR}/.."
BUILD_DIR="${REPO_DIR}/build-release"
INSTALL_DIR="${BUILD_DIR}/install-release"

# Run our build script
pushd ${SCRIPT_DIR}
./cbr-build-release.sh
popd

pushd ${BUILD_DIR}
mkdir -p ${INSTALL_DIR}
cmake --install . --prefix ${INSTALL_DIR}
popd
