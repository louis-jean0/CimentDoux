#!/bin/sh
bindir=$(pwd)
cd /home/e20190010736/HAI819I-moteurjeux/TP1/TP1/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /home/e20190010736/HAI819I-moteurjeux/TP1/build/TP1 
	else
		"/home/e20190010736/HAI819I-moteurjeux/TP1/build/TP1"  
	fi
else
	"/home/e20190010736/HAI819I-moteurjeux/TP1/build/TP1"  
fi
