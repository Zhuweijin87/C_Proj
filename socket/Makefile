cc=gcc
#gcc -w close all warnings
#	 -werror
#	 -Wall list all warnings that will occured
#	 -Wno- no warnings with
cflags=-std=c99 -Wall -Wno-implicit-function-declaration
obj_file=socket.o poll.o epoll.o
test_obj=test.o
target=libsock.a test

.PHONY :clean all

all:$(obj_file) $(target) $(test_obj)
%.o:%.c
	$(cc) $(cflags) -c $<

libsock.a:$(obj_file)
	ar rv $@ $(obj_file)

test:$(test_obj)
	$(cc) -o test $(test_obj) -L. -L$(HOME)/gitdev/C_Proj/socket -lsock

clean:
	rm -f $(obj_file) $(target) $(test_obj)
