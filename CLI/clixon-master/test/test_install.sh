#!/bin/bash
# Install test

# include err() and new() functions and creates $dir
. ./lib.sh

new "Set up installdir $dir"

new "Make DESTDIR install"
(cd ..; make DESTDIR=$dir install)
if [ $? -ne 0 ]; then
    err
fi

new "Check installed files"
if [ ! -d $dir/usr ]; then
    err $dir/usr
fi
if [ ! -d $dir/www-data ]; then
    err $dir/www-data
fi
if [ ! -f $dir/usr/local/share/clixon/clixon.mk ]; then
    err $dir/usr/local/share/clixon/clixon.mk
fi
if [ ! -f $dir/usr/local/share/clixon/clixon-config* ]; then
    err $dir/usr/local/share/clixon/clixon-config*
fi
if [ ! -h $dir/usr/local/lib/libclixon.so ]; then
    err $dir/usr/local/lib/libclixon.so
fi
if [ ! -h $dir/usr/local/lib/libclixon_backend.so ]; then
    err $dir/usr/local/lib/libclixon_backend.so
fi

new "Make DESTDIR install include"
(cd ..; make DESTDIR=$dir install-include)
if [ $? -ne 0 ]; then
    err
fi
new "Check installed includes"
if [ ! -f $dir/usr/local/include/clixon/clixon.h ]; then
    err $dir/usr/local/include/clixon/clixon.h
fi
new "Make DESTDIR uninstall"
(cd ..; make DESTDIR=$dir uninstall)
if [ $? -ne 0 ]; then
    err
fi

new "Check remaining files"
f=$(find $dir -type f)
if [ -n "$f" ]; then
    err "$f"
fi

new "Check remaining symlinks"
l=$(find $dir -type l)
if [ -n "$l" ]; then
    err "$l"
fi
