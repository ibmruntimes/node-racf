#!/bin/sh
# This script will set Node to be program controlled in order to access RACF

if [ -z $NODE_INSTALL_DIR ]; then
	echo "\$NODE_INSTALL_DIR must be set to the base install dir of NodeJS for z/OS"
	exit 1
fi

set -x
extattr +p build/Release/racf.node
if [ -e "build/Release/racf.node"  ]; then
	extattr +p "build/Release/racf.node"
fi
if [ -e "$NODE_INSTALL_DIR/lib/libnode.so" ]; then
	extattr +p "$NODE_INSTALL_DIR/lib/libnode.so"
fi
if [ -e "$NODE_INSTALL_DIR/bin/node" ]; then
	extattr +p "$NODE_INSTALL_DIR/bin/node"
fi
if [ -e CXXRT64 ]; then
	extattr +p CXXRT64
fi
