#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd ${DIR}/../

make clean;
make -j;

git status > gitversion.txt
git rev-parse HEAD >> gitversion.txt
git log >> gitversion.txt
git diff >> gitversion.txt

tar -chJf $DIR/package.tar.xz sklimmer gitversion.txt

rm gitversion.txt
