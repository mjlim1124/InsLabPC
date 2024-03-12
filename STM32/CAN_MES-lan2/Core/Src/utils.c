/*
 * utils.c
 *
 *  Created on: Dec 7, 2023
 *      Author: vanpe
 */

#include "utils.h"
#include <stdint.h>
#include <math.h>

float convertRawToTemperature(int raw1, int raw2) {
    // Implementation
	return 0.0f;
}

float convertRawToSpeed(int raw1, int raw2) {
    // Implementation
	return 0.0f;
}

float convertRawToFuel(int raw1, int raw2) {
    // Implementation
	return 0.0f;
}

int extractIntegerPart(float value) {
    return (int)floor(value);
}

int extractIntegerPart2(float value) {
    // Implementation
	return (uint8_t)value;
}

int extractDecimalPart(float value) {
    // Trừ đi phần nguyên để giữ lại phần thập phân
    float decimalPart = value - (int)value;

    // Nhân với 100 hoặc 1000 để tăng độ chính xác, tùy thuộc vào số lượng chữ số thập phân bạn muốn giữ lại
    decimalPart *= 100;

    // Ép kiểu về int và trả về kết quả
    return (int)decimalPart;
}


int extractDecimalPart2(float value) {
    // Implementation
	return (uint8_t)value;
}

