nasm -f elf test.asm
ld -m elf_i386 -s test.o io.o -o test
for i in {0..40}
do
	printf "$i" | ./test  >> output.txt
done
python ./calc.py
