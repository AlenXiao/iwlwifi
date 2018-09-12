#!/bin/sh

awss_major=`dmesg | grep "awss_major:" | grep ":[1-9]\{1,3\}" -o | grep "[1-9]*" -o`
echo $awss_major

rm -f /dev/awss
mknod /dev/awss c $awss_major 0
chmod 777 /dev/awss
