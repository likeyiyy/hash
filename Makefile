#Default Target.


OPT = -Os
CFLAGS += -std=gnu99 -Wall -g
OBJS += hash.o entity.o queue.o sysenv.o						


.PHONY:clean all

EXECS=main
	 
all:$(EXECS)

$(EXECS):main.c $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f *.o $(EXECS)

