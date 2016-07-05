CC=gcc -W
BIN_FILE=libsyutil.so

O_FILE=log.o strings.o

#if lib*.so should be compiled to 64-bit dynamic
#it will compile *.o like cc -fPIC -c *.c
all:$(BIN_FILE)
%.o:%.c
	$(CC) -fPIC -c $<

libsyutil.so:$(O_FILE)
	$(CC) -shared -fPIC -rdynamic -o $@ $?

clean:
	rm -f $(O_FILE) $(BIN_FILE)

	
