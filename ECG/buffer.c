/*
 * buffer.c
 *
 *  Created on: Sep 24, 2014
 *      Author: obel
 */

#include "buffer.h"
#include <stdlib.h>


/* This file contains functions for the buffer data structure.
 * The data structure has a fixed length in which data can be stored
 * if the structure is filled, the new data should replace the oldest
 *
 * The methods all use the buffer structure which include:
 *
 * array: the array containing the data
 *
 * length: the length of the array
 *
 * head: the index of the oldest data in the structure.
 * 		 And is the head of the structure.
 */

//Takes a length and returns a buffer with that length
Buffer makeBuffer(int length){
	Buffer newBuffer;
	newBuffer.array = malloc(length*sizeof(int));
	if (newBuffer.array==NULL){
		//TODO: gør noget hvis der ikke er plads til arrayet
		// vi vil nok bare afslutte med en exception, vi regner jo med at vi har den hukommelse vi skal bruge
	}
	// the proceding for-loop is only necessary if the compiler does not
	// set the array to 0 pr. default
	int i;
	for (i = 0 ; i<length ; i++){
		newBuffer.array[i]=0;
	}
	newBuffer.length=length;
	newBuffer.head=length-1;
	return newBuffer;
}

// inserts an element into the buffer
void insert(Buffer *buffer, int x){
	if (buffer->head==0){
		buffer->array[0] = x;
		buffer->head=buffer->length-1;
	} else {
		buffer->array[buffer->head] = x;
		buffer->head--;
	}
	//printArray(buffer);
}

// returns data in the dataset. if x = 0 the newest data X[n] is returned
// if x = 1 then X[n-1] is returned
int getPastBuffered(Buffer *buffer, int x){
	return buffer->array[(buffer->head+1+x)%(buffer->length)];
}

void printArray(Buffer buffer){
	int i;
	printf("\n{ ");
	for (i=0 ; i<buffer.length ; i++){
		printf("%d, ",buffer.array[i]);
	}
	printf(" }\n");
}
