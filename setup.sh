#!/bin/sh
# This script will set Node to be program controlled in order to access RACF

if [ -z $NODE_INSTALL_DIR ]; then
	NODE_INSTALL_DIR=`dirname $(/bin/type node | /bin/sed -e "s/node is //g")`/..
fi

set -x

find . -name "racf.node" -exec extattr +p  {} \;

for l in `ls $NODE_INSTALL_DIR/lib/libnode*.so`; do
if [ -e $l ]; then
	extattr +p $l
fi
done

if [ -e "$NODE_INSTALL_DIR/bin/node" ]; then
	extattr +p "$NODE_INSTALL_DIR/bin/node"
fi
if [ -e CXXRT64 ]; then
	extattr +p CXXRT64
fi
