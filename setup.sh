#!/bin/env bash
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


if [ -z "$NODE_RUNTIME" ]; then
  NODE_RUNTIME="$(echo $STEPLIB | tr : '\n' | grep WOZDEV | head -1)"
  if [ -z "$NODE_RUNTIME" ]; then
    exit 0;
  fi
fi
/bin/tsocmd "listcat ENT('$NODE_RUNTIME')" >/dev/null 2>/dev/null
export STEPLIB=
export LIBPATH=".:$LIBPATH"
rc=$?
if [ $rc -eq 0 ]; then
  cp "//'$NODE_RUNTIME'" .
  for file in crt* ; do mv -- "$file" "$(echo "$file" | awk '{print toupper($0)}')"; done
  extattr +p CRT*
fi
