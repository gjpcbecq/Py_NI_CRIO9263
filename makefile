NIDAQmxLib = "D:\Users\sotelo\Documents\ni\lib\NIDAQmx.lib" 
ARCHI=-m32

all : 
	gcc -c -o generateSignal.o generateSignal.c
	gcc -o generateSignal.exe generateSignal.o $(NIDAQmxLib)

library : 
	gcc -c -o ni9263.o generateSignal.c
	gcc -shared $(ARCHI) -o ni9263.lib ni9263.o $(NIDAQmxLib) 
	
clean : 
	del *.o
	del *.exe
	
testSignal : 
	gcc -c -o testSignal.o testSignal.c
	gcc -o testSignal.exe testSignal.o

