#!/bin/sh
#
# $Id: rmshm.sh,v 1.2 2008/05/29 04:54:56 yoshi Exp $
#
# Clean up shm and sem resources.
#

user=`id -un`
ids=`ipcs -m | awk "{if (\\$6 == 0 && \\$3 == \"$user\") { print \\$2;}}"`
#ids=""
for i in $ids
do
  echo "kill shm : shmid = " $i
  ipcrm -m $i
done

ids=`ipcs -s | awk "{if (\\$5 == 206 && \\$3 == \"$user\") { print \\$2;}}"`

for i in $ids
do
  echo "kill sem : semid = " $i
  ipcrm -s $i
done

