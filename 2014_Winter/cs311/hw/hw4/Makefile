#  Name: Matt Schreiber
#  Email: schreibm@onid.oregonstate.edu
#  Class: CS311-400
#  Assignment: 4

CC=gcc
DEBUG=-g
CFLAGS=$(DEBUG) -Wall --pedantic-errors -std=gnu99
PROGS=myar sig_demo
OBJECTS=$(addsuffix .o, $(PROGS))

ARQ=testq12345 testq135 testq24
ART=testt12345 testt135 testt24
ARV=testv12345 testv135 testv24
TEST=testq testt testv

OTF=1 3 5
TF=2 4
OTTFF=1 2 3 4 5
OTFTXTS=$(addsuffix -s.txt, $(OTF))
TFTXTS=$(addsuffix -s.txt, $(TF))
TXTS=$(addsuffix -s.txt, $(OTTFF))
BIG=big.txt

RM12345=-f ar12345.ar myar12345.ar 2>/dev/null
RM135=-f ar135.ar myar135.ar 2>/dev/null
RM24=-f ar24.ar myar24.ar 2>/dev/null
RMBIG=-f arbig.ar myarbig.ar 2>/dev/null

all: $(PROGS)

myar: myar.o
	$(CC) $(CFLAGS) $(DEBUG) -o $@ $@.o

myar.o: myar.c
	$(CC) $(CFLAGS) $(DEBUG) -c $^ -o $@

sig_demo: sig_demo.o
	$(CC) $(CFLAGS) -o $@ $@.o

sig_demo.o: sig_demo.c
	$(CC) $(CFLAGS) -c $^ -o $@

testq12345:
	-@rm $(RM12345)
	ar q ar12345.ar $(TXTS)
	./myar -q myar12345.ar $(TXTS)
	diff ar12345.ar myar12345.ar

testq135:
	-@rm $(RM135)
	ar q ar135.ar $(OTFTXTS)
	./myar -q myar135.ar $(OTFTXTS)
	diff ar135.ar myar135.ar

testq24:
	-@rm $(RM24)
	ar q ar24.ar $(TFTXTS)
	./myar -q myar24.ar $(TFTXTS)
	diff ar24.ar myar24.ar

testq: $(ARQ)

testt12345:
	-@rm $(RM12345)
	ar q ar12345.ar $(TXTS)
	./myar -q myar12345.ar $(TXTS)
	ar t ar12345.ar > ar-ctoc12345.txt
	./myar -t myar12345.ar > myar-ctoc12345.txt
	diff ar-ctoc12345.txt myar-ctoc12345.txt

testt135:
	-@rm $(RM135)
	ar q ar135.ar $(OTFTXTS)
	./myar -q myar135.ar $(OTFTXTS)
	ar t ar135.ar > ar-ctoc135.txt
	./myar -t myar135.ar > myar-ctoc135.txt
	diff ar-ctoc135.txt myar-ctoc135.txt

testt24:
	-@rm $(RM24)
	ar q ar24.ar $(TFTXTS)
	./myar -q myar24.ar $(TFTXTS)
	ar t ar24.ar > ar-ctoc24.txt
	./myar -t myar24.ar > myar-ctoc24.txt
	diff ar-ctoc24.txt myar-ctoc24.txt

testt: $(ART)

testv12345:
	-@rm $(RM12345)
	ar q ar12345.ar $(TXTS)
	./myar -q myar12345.ar $(TXTS)
	ar tv ar12345.ar > ar-vtoc12345.txt
	./myar -v myar12345.ar > myar-vtoc12345.txt
	diff ar-vtoc12345.txt myar-vtoc12345.txt

testv135:
	-@rm $(RM135)
	ar q ar135.ar $(OTFTXTS)
	./myar -q myar135.ar $(OTFTXTS)
	ar tv ar135.ar > ar-vtoc135.txt
	./myar -v myar135.ar > myar-vtoc135.txt
	diff ar-vtoc135.txt myar-vtoc135.txt

testv24:
	-@rm $(RM24)
	ar q ar24.ar $(TFTXTS)
	./myar -q myar24.ar $(TFTXTS)
	ar tv ar24.ar > ar-vtoc24.txt
	./myar -v myar24.ar > myar-vtoc24.txt
	diff ar-vtoc24.txt myar-vtoc24.txt

testv: $(ARV)

testbig:
	-@rm $(RMBIG)
	ar q arbig.ar $(BIG)
	./myar -q myarbig.ar $(BIG)
	ar tv arbig.ar > ar-vtocbig.txt
	./myar -v myarbig.ar > myar-vtocbig.txt
	diff ar-vtocbig.txt myar-vtocbig.txt

tests: $(TEST)

clean:
	-@rm -f $(PROGS) $(OBJECTS) *-ctoc* *-vtoc* *.ar *~ *# 2>/dev/null

