#ifndef BLE_MODULE_H
#define BLE_MODULE_H

#include <cstddef>

#include <Arduino.h>
#include <SenseBoxBLE.h>

class BLEModule
{
public:
    BLEModule();

    // Initialize the BLE module
    bool begin();

    // Get the BLE module ID
    String getBLEName();
    static unsigned int characteristicMaxLength();

    void blePoll();

    const char **getBLEConnectionString();

    static int createService(const char *uuid);

    // Create a BLE characteristic
    static int createCharacteristic(const char *uuid);
    static int createCharacteristic(
        const char *uuid, const CharacteristicProperties &properties);

    static bool writeBLE(
        int characteristicId, uint8_t const *data, std::size_t len);
    static bool writeBLE(int characteristicId, float value);
    static bool writeBLE(int characteristicId, float value, float value2);
    static bool writeBLE(int characteristicId, float value, float value2, float value3);
    static bool writeBLE(int characteristicId, float value, float value2, float value3, float value4);
    static bool writeBLE(int characteristicId, float value, float value2, float value3, float value4, float value5);

    // Set callback for receiving data
    // void setReceiveCallback(void (*callback)(BLEDevice, BLECharacteristic));

    // Task function for polling BLE
    static void bleTask(void *pvParameters);

private:
    // BLEService* service;
    String bleName;

    // static void onReceive(BLEDevice central, BLECharacteristic characteristic);
    // static void (*receiveCallback)(BLEDevice, BLECharacteristic);
};

#endif // BLE_MODULE_H
