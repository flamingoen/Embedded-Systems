/*
 * main.c
 *
 *  Created on: 10/09/2014
 *      Author: Andreas
 */

#include <stdio.h>
#include <stdlib.h>
#include "sensor.h"
#include "filter.h"
#include "detect.h"
#include "buffer.h"

// used to store old data samples
Buffer buffered_filtered_data, buffered_raw_data, buffered_LP_data, buffered_HP_data, buffered_derivative_square_data, buffered_peak_lines;
int line = 0, number=1;

int runECG();
int filter();
void detectPeaks();
int calculatePulse(int line);
void freeBuffers(Buffer* buffered_raw_data, Buffer* buffered_LP_data,
		Buffer* buffered_HP_data, Buffer* buffered_derivative_square_data,
		Buffer* buffered_filtered_data, Buffer* buffered_peak_lines);



int main() {
	return runECG();
}

/* The main function for running the ECG program. */
int runECG() {
	openFile();

	//printf("Peak nr.   Line        R peak\n");
	buffered_filtered_data = makeBuffer(SIZE_FILTERED_DATA);
	buffered_raw_data = makeBuffer(SIZE_RAW_DATA);
	buffered_LP_data = makeBuffer(SIZE_LP_DATA);
	buffered_HP_data = makeBuffer(SIZE_HP_DATA);
	buffered_derivative_square_data = makeBuffer(SIZE_DERIVATIVESQUARE_DATA);
	buffered_peak_lines = makeBuffer(SIZE_PEAKS);
	initQRS();
	while (getFilePointer() != EOF) {
		insert(&buffered_filtered_data, filter());
		detectPeaks();
		line++;
	}

	freeBuffers(&buffered_raw_data, &buffered_LP_data, &buffered_HP_data,
			&buffered_derivative_square_data, &buffered_filtered_data,
			&buffered_peak_lines);
	return 0;
}

// function containing the filtering of the data
int filter() {
	insert(&buffered_raw_data, getNextData());
	insert(&buffered_LP_data, lowPass(&buffered_raw_data, &buffered_LP_data));
	insert(&buffered_HP_data, highPass(&buffered_LP_data, &buffered_HP_data));
	insert(&buffered_derivative_square_data,
			derivative_square(&buffered_HP_data));
	return movingWindowIntegration(&buffered_derivative_square_data);
}

void detectPeaks() {

	int R_peak = run_QRS(getPastBuffered(&buffered_filtered_data, 2),
			getPastBuffered(&buffered_filtered_data, 1),
			getPastBuffered(&buffered_filtered_data, 0));

	if (R_peak) {
		printf("  %d\t", number);
		printf("%d:\t", line);
		printf("R-peak: %d\t", getPastBuffered(&buffered_filtered_data, 1));
		printf("Measured at: %.2f s\t", 0.004 * line);
		printf("Current pulse: %d BPM\n", calculatePulse(line));
		if (getPastBuffered(&buffered_filtered_data, 1) < 2000) { //TODO: fjern magisk tal!
			printf(
					"!WARNING!: PULSE LOW.\n !SEEK MEDICAL ATTENTION IMMEDIATELY!\n");
		}
		number++;
		if (R_peak == 2) {
			int missed_peak = searchBack();
			if (missed_peak) {
				printf("  %d\t", number);
				printf("%d:\t", line);
				printf("Missed R-peak: %d\t", missed_peak);
				printf("Measured at: %.2f s\t", 0.004 * line);
				//printf("Current pulse: %d BPM\n", calculatePulse(line));
				if (missed_peak < 2000) { //TODO: fjern magisk tal!
					printf(
							"!WARNING!: PULSE LOW.\n !SEEK MEDICAL ATTENTION IMMEDIATELY!\n");
				}
				number++;
			}
		}
	}



	checkMissedInterval();
}

int calculatePulse(int line) { //TODO: det her forstår jeg slet ikke - kunne man gøre det lidt ligesom med average, hvor man tager højde for at man ikke har alle punkter fra starten?
	insert(&buffered_peak_lines, line);
	line = (line - getPastBuffered(&buffered_peak_lines, SIZE_PEAKS-1))/ SIZE_PEAKS;
	return 60 / (line*0.004);
}

void freeBuffers(Buffer* buffered_raw_data, Buffer* buffered_LP_data,
		Buffer* buffered_HP_data, Buffer* buffered_derivative_square_data,
		Buffer* buffered_filtered_data, Buffer* buffered_peak_lines) {
	free(buffered_raw_data->array);
	free(buffered_LP_data->array);
	free(buffered_HP_data->array);
	free(buffered_derivative_square_data->array);
	free(buffered_filtered_data->array);
	free(buffered_peak_lines->array);
}
