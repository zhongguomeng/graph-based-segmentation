# C only implementation makefile
# MacOSX version

CC=g++

#CFLAGS=-I/usr/local/Cellar/opencv/2.4.9/include/opencv2 -L/usr/local/Cellar/opencv/2.4.9/lib -lopencv_core

CONF=`pkg-config --cflags --libs opencv`

all:
	echo $(CONF);
	$(CC) main.cpp -o main $(CONF);
