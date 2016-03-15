#!/bin/bash

PYTHON="/usr/bin/python3"

# Потом этот каталог удаляется
mkdir -p /usr/lib/uwsgi

$PYTHON uwsgiconfig.py --clean
$PYTHON uwsgiconfig.py --build ots

mkdir out
mkdir -p out/usr/bin
mkdir -p out/usr/lib/uwsgi
mkdir -p out/DEBIAN

mv control out/DEBIAN
mv uwsgi out/usr/bin
mv /usr/lib/uwsgi/*.so out/usr/lib/uwsgi

# Я ведь обещал
rm -rf /usr/lib/uwsgi

fakeroot dpkg-deb --build out

rm -rf out
mv out.deb uwsgi-2.0.12.deb
