CC = gcc
CFLAGS = -g -no-pie

# Executables to build
EXES = myCountToNIncr countToNIncr

% : %.S
	$(CC) $(CFLAGS) $*.S -o $@

all : $(EXES)

clean :
	rm -f $(EXES)
