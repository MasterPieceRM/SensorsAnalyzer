#include "structs.h"

int compare_sensors(const void *a, const void *b) {
    sensor *sensor_a = (sensor *)a;
    sensor *sensor_b = (sensor *)b;

    if (sensor_a->sensor_type == PMU && sensor_b->sensor_type == TIRE) {
        return -1;  // PMU first
    } else if (sensor_a->sensor_type == TIRE && sensor_b->sensor_type == PMU) {
        return 1;  // TIRE first
    } else {
        return 0;  // no change
    }
}

int main(int argc, char const *argv[]) {
    int nrSenzori = 0, i = 0, numarOP = 0, OK = 0, SenzoriOK = 0;
    float pressure = 0, temperature = 0, voltage = 0, power_consumption = 0,
          current = 0;
    int energy_regen = 0, energy_storage = 0, wear_level = 0,
        performance_score = 0, index = 0;
    void *operations[nrOperatii];
    char comanda[ComandaLength] = {0};
    if (argc < 2) {
        printf("Nu s-a specificat niciun fișier de intrare\n");
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if (f == NULL) {
        printf("Nu s-a putut deschide fișierul %s\n", argv[1]);
        return 1;
    }
    fread(&nrSenzori, sizeof(int), 1, f);
    sensor *SENZORI, *SENZORIclear;
    SENZORI = (sensor *)malloc(nrSenzori * sizeof(sensor));
    if (!SENZORI) {
        printf("Eroare la alocarea memoriei pentru SENZORI\n");
        return 1;
    }
    for (i = 0; i < nrSenzori; i++) {
        fread(&((SENZORI + i)->sensor_type), sizeof(enum sensor_type), 1, f);
        if (SENZORI[i].sensor_type == TIRE) {
            SENZORI[i].sensor_data = malloc(sizeof(tire_sensor));
            if (!((SENZORI + i)->sensor_data)) {
                printf(
                    "Eroare la alocarea memoriei pentru datele senzorilor\n");
                return 1;
            }
            fread(&pressure, sizeof(float), 1, f);
            ((tire_sensor *)SENZORI[i].sensor_data)->pressure = pressure;
            fread(&temperature, sizeof(float), 1, f);
            ((tire_sensor *)SENZORI[i].sensor_data)->temperature = temperature;
            fread(&wear_level, sizeof(int), 1, f);
            ((tire_sensor *)SENZORI[i].sensor_data)->wear_level = wear_level;
            fread(&performance_score, sizeof(int), 1, f);
            ((tire_sensor *)SENZORI[i].sensor_data)->performace_score =
                performance_score;
            fread(&(SENZORI[i].nr_operations), sizeof(int), 1, f);
            SENZORI[i].operations_idxs =
                (int *)malloc(SENZORI[i].nr_operations * sizeof(int));
            fread(SENZORI[i].operations_idxs, sizeof(int),
                  SENZORI[i].nr_operations, f);
        } else {
            SENZORI[i].sensor_data = malloc(sizeof(power_management_unit));
            if (!((SENZORI + i)->sensor_data)) {
                printf(
                    "Eroare la alocarea memoriei pentru datele senzorilor\n");
                return 1;
            }
            fread(&voltage, sizeof(float), 1, f);
            ((power_management_unit *)SENZORI[i].sensor_data)->voltage =
                voltage;
            fread(&current, sizeof(float), 1, f);
            ((power_management_unit *)SENZORI[i].sensor_data)->current =
                current;
            fread(&power_consumption, sizeof(float), 1, f);
            ((power_management_unit *)SENZORI[i].sensor_data)
                ->power_consumption = power_consumption;
            fread(&energy_regen, sizeof(int), 1, f);
            ((power_management_unit *)SENZORI[i].sensor_data)->energy_regen =
                energy_regen;
            fread(&energy_storage, sizeof(int), 1, f);
            ((power_management_unit *)SENZORI[i].sensor_data)->energy_storage =
                energy_storage;
            fread(&(SENZORI[i].nr_operations), sizeof(int), 1, f);
            SENZORI[i].operations_idxs =
                (int *)malloc(SENZORI[i].nr_operations * sizeof(int));
            fread(SENZORI[i].operations_idxs, sizeof(int),
                  SENZORI[i].nr_operations, f);
        }
    }
    qsort(SENZORI, nrSenzori, sizeof(sensor), compare_sensors);
    get_operations(operations);
    scanf("%s", comanda);
    while (strcmp(comanda, "exit")) {
        if (strstr(comanda, "print")) {
            scanf("%d", &index);
            if (index < 0 || index >= nrSenzori) {
                printf("Index not in range!\n");
            } else {
                if (SENZORI[index].sensor_type == TIRE) {
                    printf("Tire Sensor\n");
                    printf(
                        "Pressure: %0.2f\n",
                        ((tire_sensor *)SENZORI[index].sensor_data)->pressure);
                    printf("Temperature: %0.2f\n",
                           ((tire_sensor *)SENZORI[index].sensor_data)
                               ->temperature);
                    printf("Wear Level: %d%%\n",
                           ((tire_sensor *)SENZORI[index].sensor_data)
                               ->wear_level);
                    if (!(((tire_sensor *)SENZORI[index].sensor_data)
                              ->performace_score)) {
                        printf("Performance Score: Not Calculated\n");
                    } else {
                        printf("Performance Score: %d\n",
                               ((tire_sensor *)SENZORI[index].sensor_data)
                                   ->performace_score);
                    }
                } else {
                    printf("Power Management Unit\n");
                    printf("Voltage: %0.2f\n",
                           ((power_management_unit *)SENZORI[index].sensor_data)
                               ->voltage);
                    printf("Current: %0.2f\n",
                           ((power_management_unit *)SENZORI[index].sensor_data)
                               ->current);
                    printf("Power Consumption: %0.2f\n",
                           ((power_management_unit *)SENZORI[index].sensor_data)
                               ->power_consumption);
                    printf("Energy Regen: %d%%\n",
                           ((power_management_unit *)SENZORI[index].sensor_data)
                               ->energy_regen);
                    printf("Energy Storage: %d%%\n",
                           ((power_management_unit *)SENZORI[index].sensor_data)
                               ->energy_storage);
                }
            }
        } else if (strstr(comanda, "analyze")) {
            scanf("%d", &index);
            if (index < 0 || index >= nrSenzori) {
                printf("Index not in range!\n");
            } else {
                for (i = 0; i < SENZORI[index].nr_operations; i++) {
                    numarOP = SENZORI[index].operations_idxs[i];
                    ((void (*)(void *))operations[numarOP])(
                        SENZORI[index].sensor_data);
                }
            }
        } else if (strstr(comanda, "clear")) {
            for (i = 0; i < nrSenzori; i++) {
                OK = 0;
                if (SENZORI[i].sensor_type == TIRE) {
                    if (((tire_sensor *)SENZORI[i].sensor_data)->pressure <
                            MinPressure ||
                        ((tire_sensor *)SENZORI[i].sensor_data)->pressure >
                            MaxPressure) {
                        OK = 1;
                    }
                    if (((tire_sensor *)SENZORI[i].sensor_data)->temperature <
                            MinTemp ||
                        ((tire_sensor *)SENZORI[i].sensor_data)->temperature >
                            MaxTemp) {
                        OK = 1;
                    }
                    if (((tire_sensor *)SENZORI[i].sensor_data)->wear_level <
                            MinWLevel ||
                        ((tire_sensor *)SENZORI[i].sensor_data)->wear_level >
                            MaxWLevel) {
                        OK = 1;
                    }
                } else {
                    if (((power_management_unit *)SENZORI[i].sensor_data)
                                ->voltage < MinVoltage ||
                        ((power_management_unit *)SENZORI[i].sensor_data)
                                ->voltage > MaxVoltage) {
                        OK = 1;
                    }
                    if (((power_management_unit *)SENZORI[i].sensor_data)
                                ->current < MinCurrent ||
                        ((power_management_unit *)SENZORI[i].sensor_data)
                                ->current > MaxCurrent) {
                        OK = 1;
                    }
                    if (((power_management_unit *)SENZORI[i].sensor_data)
                                ->power_consumption < MinPowCons ||
                        ((power_management_unit *)SENZORI[i].sensor_data)
                                ->power_consumption > MaxPowCons) {
                        OK = 1;
                    }
                    if (((power_management_unit *)SENZORI[i].sensor_data)
                                ->energy_regen < MinEnRegen ||
                        ((power_management_unit *)SENZORI[i].sensor_data)
                                ->energy_regen > MaxEnRegen) {
                        OK = 1;
                    }
                    if (((power_management_unit *)SENZORI[i].sensor_data)
                                ->energy_storage < MinEnStorage ||
                        ((power_management_unit *)SENZORI[i].sensor_data)
                                ->energy_storage > MaxEnStorage) {
                        OK = 1;
                    }
                }
                if (!OK) {
                    SenzoriOK++;
                    if (SenzoriOK == 1) {
                        SENZORIclear = (sensor *)malloc(sizeof(sensor));
                    } else {
                        SENZORIclear = (sensor *)realloc(
                            SENZORIclear, SenzoriOK * sizeof(sensor));
                    }
                    SENZORIclear[SenzoriOK - 1] = SENZORI[i];
                } else {
                    free(SENZORI[i].sensor_data);
                    free(SENZORI[i].operations_idxs);
                }
            }
            nrSenzori = SenzoriOK;
            memcpy(SENZORI, SENZORIclear, SenzoriOK * sizeof(sensor));
            free(SENZORIclear);
            SENZORI = (sensor *)realloc(SENZORI, SenzoriOK * sizeof(sensor));
        }
        scanf("%s", comanda);
    }
    for (i = 0; i < nrSenzori; i++) {
        free(SENZORI[i].sensor_data);
        free(SENZORI[i].operations_idxs);
    }
    free(SENZORI);
    fclose(f);
    return 0;
}
