/*
 * filter.h
 *
 *  Created on: Sep 17, 2014
 *      Author: obel
 */

#define SIZE_RAW_DATA 13
#define SIZE_LP_DATA 33
#define SIZE_HP_DATA 5
#define SIZE_DERIVATIVESQUARE_DATA 30 //should be the same as N in the MWI filter
#define SIZE_FILTERED_DATA 1000
#define SIZE_PEAKS 10

struct buffer;

int lowPass(struct buffer *rawData, struct buffer *LPData);
int highPass(struct buffer *LPData, struct buffer *HPData);
int derivative_square(struct buffer *HPData);
int movingWindowIntegration(struct buffer *DQData);
