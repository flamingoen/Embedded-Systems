#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"

FILE *ECG;
int file_pointer = 0;

int openFile(){

	ECG = fopen("ECG.txt","r");

	return 0;
}

int getNextData() {

	int i;
	file_pointer=fscanf(ECG,"%i",&i);

	return i;
}

int getFilePointer(){
	return file_pointer;
}
/*
// TODO: remove
// the method returns the data it had to store
int bufferData (int data, int *array , int arraySize){
	int i;
	for (i = 1 ; i<arraySize ; i++){
		array[i-1]=array[i];
	}
	array[arraySize-1] = data;
	return data;
}
*/
