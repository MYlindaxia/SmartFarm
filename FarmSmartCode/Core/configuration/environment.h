//
// Created by MYlin on 2026/10/13.
//

#ifndef FARMSMARTCODE_ENVIRONMENT_H
#define FARMSMARTCODE_ENVIRONMENT_H

typedef struct {
    float temperature;
    float humidity;
    float soilTemperature;
    float illuminance;
    float precipitation;
} SensorSnapshot;

extern SensorSnapshot sensorSnapshot;

typedef struct {
    float min;
    float max;
}SafeRange;

typedef struct  {
    float MAX_TEMPERATURE;
    float MIN_TEMPERATURE;
    float MAX_HUMIDITY;
    float MIN_HUMIDITY;
    float MAX_SoilTemperature;
    float MIN_SoilTemperature;
    float MAX_Illuminance;
    float MIN_Illuminance;
    float MAX_Precipitation;
    float MIN_Precipitation;
}FarmSafeState;

extern FarmSafeState farmSafeState;

typedef enum{
    OK = 0,
    EXCEED,
    LOW
}SettingState;
typedef struct {
    SafeRange temperature;
    SafeRange humidity;
    SafeRange soilTemperature;
    SafeRange illuminance;
    SafeRange precipitation;
}UserSetState;
extern UserSetState userSetState;

SettingState SetMaxTemperature(float temperature);
SettingState SetMinTemperature(float temperature);
SettingState SetMaxHumidity(float humidity);
SettingState SetMinHumidity(float humidity);
SettingState SetMaxSoilTemperature(float soilTemperature);
SettingState SetMinSoilTemperature(float soilTemperature);
SettingState SetMaxIlluminance(float illuminance);
SettingState SetMinIlluminance(float illuminance);
SettingState SetMaxPrecipitation(float precipitation);
SettingState SetMinPrecipitation(float precipitation);


void InitFarmSafeState(void);
void InitUserSetState(void);
float GetTemperature(void);
float GetHumidity(void);
float GetSoilTemperature(void);
float GetIlluminance(void);
float GetPrecipitation(void);
#endif //FARMSMARTCODE_ENVIRONMENT_H
