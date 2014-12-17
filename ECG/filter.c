/*
 * filter.c
 *
 *  Created on: 10/09/2014
 *      Author: Andreas
 */

#include <stdio.h>
#include "filter.h"
#include "buffer.h"

// This function takes the last raw data buffered and returns the value after
// the low pass filtering have been applied
int lowPass(Buffer *rawData, Buffer *LPData) {
	// The equation for the low pass filtering
	return (2 * getPastBuffered(LPData,0) - getPastBuffered(LPData,1)
			+ (getPastBuffered(rawData,0) - 2 * getPastBuffered(rawData,6)
					+ getPastBuffered(rawData,12)) / 32);
}

// This function takes the last low pass data buffered and returns the value after
// the high pass filtering have been applied
int highPass(Buffer *LPData, Buffer *HPData) {
	// The equation for the high pass filtering
	return getPastBuffered(HPData,0) - getPastBuffered(LPData,0) / 32
			+ getPastBuffered(LPData,16) - getPastBuffered(LPData,17)
			+ getPastBuffered(LPData,32) / 32;
}

/* this function takes the high pass data and returns the value after the derivative
 *  and square filter have been applied */
int derivative_square(Buffer *HPData) {

	// TODO: find out if this is faster than simply running the equation twice
	int y;

	// The equation for the derivative filtering
	y = (2 * getPastBuffered(HPData,0) + getPastBuffered(HPData,1)
			- getPastBuffered(HPData,3) - 2 * getPastBuffered(HPData,4)) / 8;


	// returns the squared value
	return y*y;
}

/* this function takes the squared data from the derivative filter and returns the value
 *  after the moving window integration have been applied */
int movingWindowIntegration(Buffer *DQData){

	int y = 0;
	int i;

	// calculates the sum of the upper part of the equation
	for (i = 0 ; i<SIZE_DERIVATIVESQUARE_DATA ; i++){
		y += getPastBuffered(DQData,i);
	}

	return y/SIZE_DERIVATIVESQUARE_DATA;
}
