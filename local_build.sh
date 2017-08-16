#!/bin/bash

exec 2>&1

BUILD_ROOT='../../../'
export GCC_HOME=/opt/compiler/gcc-4.8.2/
export TOOLCHAIN_DIR=$GCC_HOME/bin
export PATH=/opt/tool/broc/client:$PATH

#broc build
broc init
[ $? -ne 0 ] && {
    echo 'broc init failed!!!'
    exit 1
}

broc build
[ $? -ne 0 ] && {
    echo 'broc build failed!!!'
    exit 1
}

#blade build
if [ ! -f $BUILD_ROOT/BLADE_ROOT ];then
    echo 'update BLADE_ROOT'
    cp $BUILD_ROOT/waimai/bi_arch/blade/waimai_blade/BLADE_ROOT $BUILD_ROOT
fi

blade build -m64
[ $? -ne 0 ] && {
    echo 'blade build failed!!!'
    exit 1
}

#output
if [ -d output ];then
    rm -rf output
fi
mkdir -p output/lib
mkdir -p output/bin

cp $BUILD_ROOT/build64_release/waimai/bi_public/benchmark/*.a output/lib
cp $BUILD_ROOT/build64_release/waimai/bi_public/benchmark/example output/bin

find ./output -name .git | xargs rm -rf
cd output
exit 0
