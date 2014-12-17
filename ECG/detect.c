/*
 * detect.c
 *
 *  Created on: Sep 17, 2014
 *      Author: obel
 */

#include <stdio.h>
#include "detect.h"
#include "sensor.h"
#include "buffer.h"

Buffer buffered_peaks;
Buffer recentRR_ok;
Buffer recentRR;

int NPKF = 2500;
int threshold1 = 3000;
int threshold2 = 0, SPKF = 0, RR_low = 0, RR_high = 0, RR_miss = 0,
		RR_average1 = 0, RR_average2 = 0, RR_miss_count = 0;
int RR_interval = 0; // a signal comes every 0.2 s, but the computer doesn't care, so an interval is just 1

int average(Buffer *buffer) {
	int temp = 0;
	int i;
	int j = 0;

	for (i = 0; i < buffer->length; i++) {
		if (buffer->array[i] != 0) {
			temp += buffer->array[i];
			j++;
		}
	}
	return temp / j;
}

void setLHM( avg) {
	RR_low = 0.92 * avg;
	RR_high = 1.16 * avg;
	RR_miss = 1.66 * avg;
}

void setThreshold() {
	threshold1 = (NPKF + 0.25 * (SPKF - NPKF));
	threshold2 = 0.5 * threshold1;
}

// TODO: Den her metode skal laves! vi printer bare ikke det vi finder i search back? virkelig?!
// Jeg gør ikke noget ved den nu
int searchBack() {
	int i;
	for (i = 1; i < SIZE_BUFFERED_PEAKS; i++) {
		if (getPastBuffered(&buffered_peaks, i) > threshold2) {
			SPKF = 0.25 * getPastBuffered(&buffered_peaks, i) + 0.75 * SPKF;
			insert(&recentRR, RR_interval);
			RR_average1 = average(&recentRR);
			setLHM(RR_average1);
			setThreshold();
			RR_interval = 0;
			/* TODO: remove
			 printf("  SB \t|   ");
			 printf("SB:\t\t");
			 printf("%d", getPastBuffered(buffered_peaks, i));
			 printf("\n");
			 */

			return getPastBuffered(&buffered_peaks, i);
		}
	}
	return 0;
}

int peakInInterval() {
	return RR_low < RR_interval && RR_interval < RR_high;
}

void setVariablesRPeakOK(R_peak) {
	SPKF = 0.125 * R_peak + 0.875 * SPKF;
	insert(&recentRR_ok, RR_interval);
	insert(&recentRR, RR_interval);
	RR_average1 = average(&recentRR);
	RR_average2 = average(&recentRR_ok);
	setLHM(RR_average2);
	setThreshold();
	RR_interval = 0;
	RR_miss_count = 0;
}

int checkRRInterval(int peak) {
	//printf("%d\t%d\t%d\n",RR_low,RR_interval,RR_high); TODO: remove
	if (peakInInterval()) {
		setVariablesRPeakOK(peak);
		return 1;
	} else {
		RR_miss_count++;

		if (RR_interval > RR_miss) {
			return 2; // return next function

		}
	}
	return 0;
}

int checkRPeak(int data) {
	insert(&buffered_peaks, data);

	if (data > threshold1) {
		return checkRRInterval(data); // return next function
	} //else if (data < 2000) {//TODO: det her er kun peaks ikke R-peaks, jeg flytter den ind i main //TODO remove
	//	printf("!WARNING!: PULSE LOW.\n !SEEK MEDICAL ATTENTION IMMEDIATELY!\n");
	 // }
	NPKF = 0.125 * data + 0.875 * NPKF;
	setThreshold();
	RR_interval++;
	return 0;
}

int dataIsPeak(int beforeData, int data, int afterData) {
	return beforeData < data && data > afterData;
}

int run_QRS(int beforeData, int data, int afterData) {
	if (dataIsPeak(beforeData, data, afterData)) {
		return checkRPeak(data);
	} else {
		RR_interval++;
		return 0;
	}
}

void initQRS() {
	buffered_peaks = makeBuffer(SIZE_BUFFERED_PEAKS);
	recentRR = makeBuffer(SIZE_RECENT);
	recentRR_ok = makeBuffer(SIZE_RECENT);
}

checkMissedInterval(){
	if (RR_miss_count > 4) {
		printf("!WARNING!: HEARTBEAT NOT DETECTED.\n !SEEK MEDICAL ATTENTION IMMEDIATELY!\n"); //TODO lav en metode der tager en warning og printer den - copy-paste = dårlig stil
		RR_miss_count = 0;
	}
}
