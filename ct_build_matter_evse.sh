#!/bin/bash

SET_TARGET=${SET_TARGET:-0}
BUILD=${BUILD:-0}
FLASH_APP=${FLASH_APP:-0}

# Fork-only patches to connectedhomeip/ (e.g. mode-base reporting). Idempotent: safe to run every time.
REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
if [[ -x "${REPO_ROOT}/patches/apply_connectedhomeip_patches.sh" ]]; then
	"${REPO_ROOT}/patches/apply_connectedhomeip_patches.sh" || exit 1
fi

#assume you have setup the env and matter path
pushd examples/energy_evse/ > /dev/null

if [ $SET_TARGET -eq 1 ]; then
    idf.py set-target esp32s3 -DCMAKE_POLICY_VERSION_MINIMUM=3.5 > /dev/null
fi

if [ $BUILD -eq 1 ]; then
    idf.py build 2>&1 | grep -E "warning|error|Project build complete"
    # idf.py build
fi

if [ $FLASH_APP -eq 1 ]; then
    idf.py app-flash
fi

popd > /dev/null
