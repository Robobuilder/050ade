CC		= gcc

CFLAGS		= -g -Wall -pedantic

LDFLAGS		= -g

OUT		= lscat lspipe

OBJS		= lscat.o lspipe.o
 
main: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)
	./lscat ./lspipe

clean:
	rm -f *.o sh core out
