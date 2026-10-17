//
// Created by MYlin on 2026/10/13.
//
#include "environment.h"
UserSetState userSetState;

SettingState SetTemperature(float temperature) {
    if (temperature > MAX_TEMPERATURE) {
        return EXCEED;
    }else if (temperature < MIN_TEMPERATURE) {
        return LOW;
    }
    userSetState.temperature = temperature;
    return OK;
}

SettingState SetHumidity(float humidity) {
    if (humidity > MAX_HUMIDITY) {
        return EXCEED;
    }else if (humidity < MIN_HUMIDITY) {
        return LOW;
    }
    userSetState.humidity = humidity;
    return OK;
}

SettingState SetSoilTemperature(float soilTemperature) {
    if (soilTemperature > MAX_SoilTemperature) {
        return EXCEED;
    }else if (soilTemperature < MIN_SoilTemperature) {
        return LOW;
    }
    userSetState.soilTemperature = soilTemperature;
    return OK;
}
SettingState SetIlluminance(float illuminance) {
    if (illuminance > MAX_Illuminance) {
        return EXCEED;
    }else if (illuminance < MIN_Illuminance) {
        return LOW;
    }
    userSetState.illuminance = illuminance;
    return OK;
}
SettingState SetPrecipitation(float precipitation) {
    if (precipitation > MAX_Precipitation) {
        return EXCEED;
    }else if (precipitation < MIN_Precipitation) {
        return LOW;
    }
    userSetState.precipitation = precipitation;
    return OK;
}
float GetTemperature() {
    return userSetState.temperature;
}
float GetHumidity() {
    return userSetState.humidity;
}
float GetSoilTemperature() {
    return userSetState.soilTemperature;
}
float GetIlluminance() {
    return userSetState.illuminance;
}
float GetPrecipitation() {
    return userSetState.precipitation;
}
