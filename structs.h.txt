#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define nrOperatii 8
#define ComandaLength 10
#define MinPressure 19
#define MaxPressure 28
#define MinTemp 0
#define MaxTemp 120
#define MinWLevel 0
#define MaxWLevel 100
#define MinVoltage 10
#define MaxVoltage 20
#define MinCurrent -100
#define MaxCurrent 100
#define MinPowCons 0
#define MaxPowCons 1000
#define MinEnRegen 0
#define MaxEnRegen 100
#define MinEnStorage 0
#define MaxEnStorage 100

enum sensor_type { TIRE, PMU };

typedef struct {
    enum sensor_type sensor_type;
    void* sensor_data;
    int nr_operations;
    int* operations_idxs;
} sensor;

typedef struct __attribute__((__packed__)) {
    float voltage;
    float current;
    float power_consumption;
    int energy_regen;
    int energy_storage;
} power_management_unit;

typedef struct __attribute__((__packed__)) {
    float pressure;
    float temperature;
    int wear_level;
    int performace_score;
} tire_sensor;
void get_operations(void** operations);
#endif  // _STRUCTS_H_