#!/bin/bash

PYTHON="/usr/bin/python3"

# Потом этот каталог удаляется
mkdir -p /usr/lib/uwsgi

$PYTHON uwsgiconfig.py --clean
$PYTHON uwsgiconfig.py --build ots

mkdir out
mkdir -p out/usr/bin
mkdir -p out/usr/lib/uwsgi
mkdir -p out/etc/uwsgi/vassals/common
mkdir -p out/lib/systemd/system
mkdir -p out/etc/systemd/system

cp -r ots/DEBIAN out
cp ots/uwsgi.service out/lib/systemd/system
cp ots/emperor.ini out/etc/uwsgi
cp ots/defaults.ini out/etc/uwsgi/vassals/common
mv uwsgi out/usr/bin
mv /usr/lib/uwsgi/*.so out/usr/lib/uwsgi

# Я ведь обещал
rm -rf /usr/lib/uwsgi

fakeroot dpkg-deb --build out

rm -rf out
mv out.deb uwsgi-dev.deb
