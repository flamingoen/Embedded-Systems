/*
 * detect.h
 *
 *  Created on: Sep 17, 2014
 *      Author: obel
 */

#define SIZE_STORED_PEAKS 1000
#define SIZE_RECENT 8
#define SIZE_BUFFERED_PEAKS 1000

int run_QRS(int beforeData, int data, int afterData);
int searchBack();
void initQRS();
void checkMissedInterval();
