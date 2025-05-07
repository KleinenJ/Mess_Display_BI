#ifndef MODEL_HPP
#define MODEL_HPP


#include <stdint.h>
#include "FLASH_SECTOR_H7.h"
#include <cstdint>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    ModelListener* getModelListener() {
            return modelListener;
        }

    void fnSendFloatsViaCAN(uint32_t uiCanID, float fValue1, float fValue2, float fValue3, float fValue4);
    void tick();
    void setBacklightValue(int value);
    void process_CAN_messages();
    void sendLinDataOverCAN();
    void sendLINControlFrame(uint8_t hv_on, uint8_t regulator, uint8_t current, uint8_t voltage, uint8_t blower);
    void receiveLINStatusFrame();
    //void startLINReception();
    //void processReceivedLINData();
    //uint8_t linRxBuffer[9];
    //void notifyLINStatusReceived(const uint8_t* data);
    //uint8_t getHviValue();
    //uint8_t Flash_ReadHvi();
    void processLINData(uint8_t* data, uint16_t size);
    //void Flash_Read_Data(uint32_t StartSectorAddress, uint32_t* data, uint16_t numberofwords);
    //uint32_t GetSector(uint32_t Address);
    //void Flash_WriteHvi(uint8_t value);
    bool validateLINData(uint8_t *data);
    void rearrangeRxData(uint8_t* data, uint8_t size);
    int ReadValueFromEEPORM();
    void SendVlaueToEEPROM(int value);
    int ReadCountEEPORM();
    bool getSDCS();
    void updatefLinIonSetCurrent(int value);
    void sendSystemImageOverCAN();

protected:
    ModelListener* modelListener;

private:
    uint8_t checksum_Calc(uint8_t PID, uint8_t *data, uint8_t size);
    uint8_t pid_Calc(uint8_t ID);
    uint32_t tickCounter;
    //uint8_t hviValue;
    //uint32_t Rx[6]; // Beispiel für empfangene Daten
    uint8_t receiveInitialHviValue();  // Neue Methode deklarieren
};

#endif // MODEL_HPP
