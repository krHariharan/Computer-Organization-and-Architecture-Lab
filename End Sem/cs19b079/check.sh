#!/bin/bash
if [ $# -ne 1 ]
then
	echo "Usage: bash check.sh program.asm"
	exit
fi

if [ ! -d testcases ]
then
	echo "testcases directory missing"
	echo "Please execute this script in the directory containing the testcases directory"
	exit
fi

if [ ! -f testcases/io.mac ]
then
	echo "io.mac file missing in testcases folder"
	exit
fi

if [ ! -f testcases/io.o ]
then
	echo "io.o file missing in testcases folder"
	exit
fi

nasm -f elf -i testcases/ $1 -o /tmp/temp.o
if [ $? -ne 0 ]
then
	echo "Assembling with nasm failed"
	exit
fi

ld -m elf_i386 -s /tmp/temp.o testcases/io.o -o output_binary
if [ $? -ne 0 ]
then
	echo "Linking with ld failed"
	exit
fi
rm /tmp/temp.o

echo "Compilation successful"

num_testcases=`/usr/bin/ls testcases/t*.in | wc -l`

testcase=1
while [ $testcase -le $num_testcases ]
do
	echo -n "Evaluating testcase $testcase: "
	# We need to pad the lines with whitespace for GetLInt macro to work properly
	cat testcases/t$testcase.in | awk '{printf("%-255s\n",$0)}' | ./output_binary > /tmp/output
	if [ `diff -b /tmp/output testcases/t$testcase.out | wc -l` -eq 0 ]
	then
		echo Passed
	else
		echo Failed
	fi
	testcase=$(($testcase+1))
done
