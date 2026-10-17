//
// Created by MYlin on 2026/10/13.
//

#ifndef FARMSMARTCODE_ENVIRONMENT_H
#define FARMSMARTCODE_ENVIRONMENT_H

#define MAX_TEMPERATURE 100.0f
#define MIN_TEMPERATURE -100.0f
#define MAX_HUMIDITY 100.0f
#define MIN_HUMIDITY -100.0f
#define MAX_SoilTemperature 100.0f
#define MIN_SoilTemperature -100.0f
#define MAX_Illuminance 100.0f
#define MIN_Illuminance -100.0f
#define MAX_Precipitation 100.0f
#define MIN_Precipitation -100.0f

typedef enum{
    OK = 0,
    EXCEED,
    LOW
}SettingState;
typedef struct {
    float temperature;
    float humidity;
    float soilTemperature;
    float illuminance;
    float precipitation;
}UserSetState;
extern UserSetState userSetState;

SettingState SetTemperature(float temperature);
SettingState SetHumidity(float humidity);
SettingState SetSoilTemperature(float soilTemperature);
SettingState SetIlluminance(float illuminance);
SettingState SetPrecipitation(float precipitation);
float GetTemperature(void);
float GetHumidity(void);
float GetSoilTemperature(void);
float GetIlluminance(void);
float GetPrecipitation(void);
#endif //FARMSMARTCODE_ENVIRONMENT_H
