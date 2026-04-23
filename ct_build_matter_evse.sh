#!/bin/bash

SET_TARGET=${SET_TARGET:-0}
BUILD=${BUILD:-0}
FLASH_APP=${FLASH_APP:-0}

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
