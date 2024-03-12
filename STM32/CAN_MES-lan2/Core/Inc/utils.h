#ifndef UTILS_H
#define UTILS_H

float convertRawToTemperature(int raw1, int raw2);
float convertRawToSpeed(int raw1, int raw2);
float convertRawToFuel(int raw1, int raw2);
int extractIntegerPart(float value);
int extractDecimalPart(float value);

#endif /* UTILS_H */

