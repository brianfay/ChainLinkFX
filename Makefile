# compilers
JCC = javac
CC = gcc

# java and c flags
JFLAGS = -d bin/ -Xlint
CFLAGS = -Wall -shared -fPIC -lm -lasound -lrt -ljack -lpthread -lportaudio

# gcc include
INCLUDE = -Iinclude/ -I/lib/jvm/java/include -I/lib/jvm/java/include/linux

java: 
	$(JCC) $(JFLAGS) src/*.java

header:
C:
	$(CC) $(CFLAGS) $(INCLUDE) -o lib/libChainLinkFX.so src/*.c

clean:
	$(RM) -R lib/* bin/* include/ChainLinkFX_JNIBridge.h
