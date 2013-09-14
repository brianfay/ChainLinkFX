#!/bin/sh

cd src
javac -d ../bin *.java
cd ../bin
java ChainLinkFX.ChainLinkFX
