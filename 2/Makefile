all: clean main # вызывается по умолчанию

main: main.o dict.o lib.o  # зависимости
	ld -o main main.o dict.o lib.o

main.o: main.asm colon.inc words.inc # проверяется на наличие
	nasm -g -felf64 main.asm -o main.o

lib.o: lib.asm
	nasm -g -felf64 lib.asm

dict.o: dict.asm
	nasm -g -felf64 dict.asm

clean: # удаляет объектные файлы командой ( make Makefile clean )
	rm -f *.o main