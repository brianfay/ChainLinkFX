#!/bin/sh

make clean
make java
cd bin
javah -d ../include/ ChainLinkFX.JNIBridge
cd ../
make C
cd bin
java -Djava.library.path=../lib ChainLinkFX.ChainLinkFX
