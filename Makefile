link: entry.o main.o script.o vm.o
	ld -Map output.map -m i386pe -o main.exe -T linker.ld entry.o main.o script.o vm.o
	objcopy -O binary main.exe main.img

entry.o: entry/entry.asm
	nasm -f elf32 entry/entry.asm -o entry.o

main.o: vm/main.c
	gcc -Os -c -ggdb3 -m16 -ffreestanding -Wno-pointer-to-int-cast -fno-PIE -nostartfiles -nostdlib -Qn -std=c99 vm/main.c -o main.o

vm.o: vm/vm.c
	gcc -Os -c -ggdb3 -m16 -ffreestanding -Wno-pointer-to-int-cast -fno-PIE -nostartfiles -nostdlib -Qn -std=c99 vm/vm.c -o vm.o

script.o: 16scriptbinary/script.asm 16scriptbinary/script.bin
	nasm -f elf32 16scriptbinary/script.asm -o script.o