#!/usr/bin/env bash

PYINFORM=/path/to/PyInform/repo
INFORM=/path/to/Inform/repo

cd $INFORM
chmod +x dist/package.sh
./dist/package.sh
mv dist/inform-1.0.1_linux-x86_64.tar.gz $PYINFORM/pyinform

cd $PYINFORM/pyinform
if [[ -d inform-1.0.1 ]]; then
    rm -r inform-1.0.1
fi
tar xzf inform-1.0.1_linux-x86_64.tar.gz
cd inform-1.0.1/lib
mkdir linux-x86_64
mv libinform* linux-x86_64

cd $PYINFORM
pip3 uninstall -y pyinform
pip3 install --user .
