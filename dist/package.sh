#!/usr/bin/env bash

version=$(cmake -H. -Bbuild/production -DCMAKE_BUILD_TYPE=Release | grep "\-\- inform version:")
version=${version#"-- inform version: "}

target=inform-$version
prefix=dist/$target

cmake -H. -Bbuild/production -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$prefix > /dev/null
make -C build/production all test
make -C build/production install > /dev/null
cp dist/install.sh $prefix > /dev/null
cp LICENSE $prefix > /dev/null
cp README.adoc $prefix > /dev/null
cp CHANGELOG.md $prefix > /dev/null

system=`uname -s`
hardware=`uname -m`

if [[ "$system" == "Darwin" ]]; then
    platform="macosx"
elif [[ "$system" == "Linux" ]]; then
    platform="linux"
else
    echo "unsupported platform: $system"
fi

tarball=$target"_"$platform"-"$hardware".tar.gz"

cd dist
echo "Creating tarball: dist/$tarball"
tar czf $tarball $target
