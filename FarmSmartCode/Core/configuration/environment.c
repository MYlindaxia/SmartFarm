//
// Created by MYlin on 2026/10/13.
//
#include "environment.h"
FarmSafeState farmSafeState;
UserSetState userSetState;
SensorSnapshot sensorSnapshot;

void InitFarmSafeState() {
    farmSafeState.MAX_HUMIDITY = 100.0f;
    farmSafeState.MIN_HUMIDITY = 0.0f;
    farmSafeState.MAX_SoilTemperature = 100.0f;
    farmSafeState.MIN_SoilTemperature = 0.0f;
    farmSafeState.MAX_Illuminance = 4000.0f;
    farmSafeState.MIN_Illuminance = 0.0f;
    farmSafeState.MAX_Precipitation = 100.0f;
    farmSafeState.MIN_Precipitation = 0.0f;
    farmSafeState.MAX_TEMPERATURE = 100.0f;
    farmSafeState.MIN_TEMPERATURE = -100.0f;
}

void InitUserSetState() {
    userSetState.temperature.max = 100.0f;
    userSetState.temperature.min = 0.0f;
    userSetState.humidity.max = 100.0f;
    userSetState.humidity.min = 0.0f;
    userSetState.soilTemperature.max = 100.0f;
    userSetState.soilTemperature.min = 0.0f;
    userSetState.illuminance.max = 2000.0f;
    userSetState.illuminance.min = 0.0f;
    userSetState.precipitation.min = 0.0f;
    userSetState.precipitation.max = 100.0f;
}

float GetTemperature(void) {
    return sensorSnapshot.temperature;
}
float GetHumidity(void) {
    return sensorSnapshot.humidity;
}
float GetSoilTemperature(void) {
    return sensorSnapshot.soilTemperature;
}
float GetIlluminance(void) {
    return sensorSnapshot.illuminance;
}
float GetPrecipitation(void) {
    return sensorSnapshot.precipitation;
}

SettingState SetMaxTemperature(float temperature) {
    if (temperature > farmSafeState.MAX_TEMPERATURE) {
        return EXCEED;
    }else if (temperature < farmSafeState.MIN_TEMPERATURE) {
        return LOW;
    }
    userSetState.temperature.max = temperature;
    return OK;
}
SettingState SetMinTemperature(float temperature) {
    if (temperature > farmSafeState.MAX_TEMPERATURE) {
        return EXCEED;
    }else if (temperature < farmSafeState.MIN_TEMPERATURE) {
        return LOW;
    }
    userSetState.temperature.min = temperature;
    return OK;
}
SettingState SetMaxHumidity(float humidity) {
    if (humidity > farmSafeState.MAX_HUMIDITY) {
        return EXCEED;
    }else if (humidity < farmSafeState.MIN_HUMIDITY) {
        return LOW;
    }
    userSetState.humidity.max = humidity;
    return OK;
}
SettingState SetMinHumidity(float humidity) {
    if (humidity > farmSafeState.MAX_HUMIDITY) {
        return EXCEED;
    }else if (humidity < farmSafeState.MIN_HUMIDITY) {
        return LOW;
    }
    userSetState.humidity.min = humidity;
    return OK;
}
SettingState SetMaxSoilTemperature(float soilTemperature) {
    if (soilTemperature > farmSafeState.MAX_SoilTemperature) {
        return EXCEED;
    }else if (soilTemperature < farmSafeState.MAX_SoilTemperature) {
        return LOW;
    }
    userSetState.soilTemperature.max = soilTemperature;
    return OK;
}
SettingState SetMinSoilTemperature(float soilTemperature) {
    if (soilTemperature > farmSafeState.MAX_SoilTemperature) {
        return EXCEED;
    }else if (soilTemperature < farmSafeState.MIN_SoilTemperature) {
        return LOW;
    }
    userSetState.soilTemperature.min = soilTemperature;
    return OK;
}
SettingState SetMaxIlluminance(float illuminance) {
    if (illuminance > farmSafeState.MAX_Illuminance) {
        return EXCEED;
    }else if (illuminance < farmSafeState.MIN_Illuminance) {
        return LOW;
    }
    userSetState.illuminance.max = illuminance;
    return OK;
}
SettingState SetMinIlluminance(float illuminance) {
    if (illuminance > farmSafeState.MAX_Illuminance) {
        return EXCEED;
    }else if (illuminance < farmSafeState.MIN_Illuminance) {
        return LOW;
    }
    userSetState.illuminance.min = illuminance;
    return OK;
}
SettingState SetMaxPrecipitation(float precipitation) {
    if (precipitation > farmSafeState.MAX_Precipitation) {
        return EXCEED;
    }else if (precipitation < farmSafeState.MIN_Precipitation) {
        return LOW;
    }
    userSetState.precipitation.max = precipitation;
    return OK;
}
SettingState SetMinPrecipitation(float precipitation) {
    if (precipitation > farmSafeState.MAX_Precipitation) {
        return EXCEED;
    }else if (precipitation < farmSafeState.MIN_Precipitation) {
        return LOW;
    }
    userSetState.precipitation.min = precipitation;
    return OK;
}


