AR=ar
CC=gcc
DEL=rm -rf
MK=mkdir
OUT=objs
RM=rmdir /s /q
CFLAGS=-Wall -O3 -DCOMPILED_FROM_DSP -DMINGW
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

EXE=yuv.exe
LIBS=yuv.a

all: $(LIBS)

$(EXE): $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^
	-$(DEL) *.a

$(LIBS): $(OBJS)
	$(AR) -r $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	-$(DEL) *.a
	-$(DEL) *.o

rebuild: clean all