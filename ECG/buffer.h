/*
 * buffer.h
 *
 *  Created on: Sep 24, 2014
 *      Author: obel
 */

struct buffer{
	int length;
	int head;
	int *array;
};

typedef struct buffer Buffer;

struct buffer makeBuffer(int length);
void insert(Buffer *buffer, int x);
int getPastBuffered(Buffer *buffer, int x);
void printArray(Buffer buffer);
