all: build

build:
	gcc -ggdb main.c controller_client.c -o main.exe -lncurses -pthread

clean:
	@rm -f main.exe

testip:
	@gcc get_ip.c
	@./a.out
	@rm a.out
