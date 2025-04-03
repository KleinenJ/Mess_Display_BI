#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "edt_bsp_can.h"
#include <stdint.h>
#include "usart.h"
#include <cstdint>
#include <cstring>
#include "FLASH_SECTOR_H7.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_flash.h"
#include "stm32h7xx_hal_flash_ex.h"
#include "fatfs.h"
#include <cstdio>

#include "edt_h7xx_eeprom_i2c.h"



// Globale Variablen für SD
//FATFS fs;    // Dateisystem
//FIL file;    // Dateiobjekt
//UINT bytesWritten;
//char buffer[100];
//FRESULT res;
//uint8_t wtext[] = "Test \n";
bool state_SDCS = false;





const uint32_t CAN_TIMEOUT_THRESHOLD = 500;		// CAN Timeout-Schwellenwert (500 Ticks)
uint32_t canTimeoutCounters[4] = {0, 0, 0, 0};	// Timeout-Zähler für die vier CAN-IDs 0x46, 0x47, 0x50, 0x51
const uint32_t LIN_TIMEOUT_THRESHOLD = 50;
uint32_t linTimeoutCounter = 0;  // Timeout-Zähler für LIN-Daten
uint16_t lastSize = 0;  		// Globale Variable, um den Wert von Size zu speichern
uint8_t linStatus[22];
//uint8_t linRxBuffer[256];

//uint8_t rxBuffer[9]; // Buffer für den Empfang
//uint8_t status[9];   // Buffer für den Status
uint8_t RxData[22];
uint8_t tempRxData[22];

//uint32_t Rx_Mem[1];
//uint32_t Tx_Mem[] = {0x28};
uint8_t tickCounter = 0;

//#define SD_CS_GPIO_Port GPIOA  // Port A für den CS-Pin (PA15)
//#define SD_CS_Pin GPIO_PIN_15  // Pin 15 als CS-Pin

Model* modelInstance = nullptr; // Globale Instanz von Model


Model::Model() : modelListener(0)//, hviValue(0)
{

	modelInstance = this;

    uint8_t TxData[2] = {0x55, pid_Calc(0x10)}; // LIN-PID 0x10
    HAL_LIN_SendBreak(&huart6); 				// LIN Break senden
    HAL_UART_Transmit(&huart6, TxData, 2, 100); // Sync Byte und PID für Statusabfrage senden

}

void Model::tick()
{
	process_CAN_messages();

    linTimeoutCounter++;		 // LIN-Timeout-Zähler hochzählen

	const uint32_t tickThreshold = 75;
	tickCounter++;

	    if (tickCounter >= tickThreshold)	        	// Anfrage an den HV-Slave nach 75 Ticks senden
	    {
	        uint8_t TxData[2] = {0x55, pid_Calc(0x10)}; // LIN-PID 0x10
	        HAL_LIN_SendBreak(&huart6); 				// LIN Break senden
	        HAL_UART_Transmit(&huart6, TxData, 2, 100); // Sync Byte und PID für Statusabfrage senden
	        tickCounter = 0;
	    }

	   HAL_UARTEx_ReceiveToIdle_IT(&huart6, RxData, 22);

	    if (RxData[10] != 0)  // Prüfen auf Datenempfang
	    {
	       __disable_irq();					// Interrupts deaktivieren
	       memcpy(tempRxData, RxData, 22);	// Kopiere RxData in den temporären Puffer
	       rearrangeRxData(tempRxData, 22);	// Werte für übergabe umsortieren (LIN-Ausfall prüfen)
	       __enable_irq();					// Interrupts wieder aktivieren
	    }

	    if (lastSize == 12)
	    {
	    modelListener->onLINStatusReceived(tempRxData);
	    linTimeoutCounter = 0;
	    }
	    else if (linTimeoutCounter > LIN_TIMEOUT_THRESHOLD)	// Timeout erkannt = keine Daten
	    {
	    linStatus[22] = {0};
	    __disable_irq();
	    memset(RxData, 0, sizeof(RxData));
	    memset(tempRxData, 0, sizeof(tempRxData));
	    __enable_irq();
	    modelListener->onLINStatusReceived(tempRxData);		// 0 Senden wenn keine Daten mehr empfangen werden
	    linTimeoutCounter = 0;  							// Timeout-Zähler zurücksetzen
	    }

	    sendLinDataOverCAN();


	   	state_SDCS = getSDCS();
	   	modelListener->setStateSDCS(state_SDCS);

}


int Model::ReadValueFromEEPORM()
{
  return EE_Read_PWM();
}


void Model::SendVlaueToEEPROM(int value)
{
  EE_Write_PWM(value);
}


int Model::ReadCountEEPORM()
{
  return EE_returnCount();
}



void Model::setBacklightValue(int value)	// Displayhelligkeit
{
	EDT_LCD_BL_SetPwm(value);

	FATFS SDFatFS; 	//Fatfs handle
		  FIL SDFile; 		//File handle
		  FRESULT fresult; //Result after operations
		  char myFilename[] = "Text1.txt";
		  char myData = value;
		//uint8_t wtext[] = "This is working with FatFs + SPI \n";
		 // UINT myBytes;
		BYTE readBuf[30];


		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

		fresult = f_mount(&SDFatFS, "", 1); //1=mount now
		  if (fresult != FR_OK)
		  {

		  }
		  fresult = f_open(&SDFile, "text.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
		    if(fresult == FR_OK)
		    {

		    }

		    strncpy((char*)readBuf, "a new file is made!", 19);
		    //readBuf[18] = '\0';
		    UINT bytesWrote;
		    //fputs("Hallo Welt", &SDFile);
		    //const char* text = "Hallo Welt";
		    fresult = f_write(&SDFile, readBuf, 19, &bytesWrote);
		     if(fresult == FR_OK)
		     {

		     }

		     f_close(&SDFile);
		     f_mount(NULL, "", 0);

		     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}


bool Model::getSDCS()
{
	GPIO_PinState readState = HAL_GPIO_ReadPin(SPI_CS_GPIO_Port, SPI_CS_Pin);
	if (readState == GPIO_PIN_SET)
	{
		return true;
	}
	else
	{
		return false;
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port, SPI_CS_Pin, GPIO_PIN_SET);
	}
}


void Model::process_CAN_messages()
{
	extern FDCAN_HandleTypeDef hfdcan1;
	uint32_t id;
	char CRxData[10];
	int16_t intData[4];
	bool idReceived[4] = {false, false, false, false};			// Array, um empfangene IDs zu markieren

	if (fdcanSt.CAN_HandleDataEn)
	  {
	    EDT_FDCANx_Receive_Msg(&hfdcan1, (uint8_t*)CRxData, 8);	// CAN-Daten empfangen
	    id = fdcanSt.IDentify;
	    fdcanSt.CAN_HandleDataEn = 0;
        for (int i = 0; i < 4; i++)
        {
            intData[i] = (int16_t)((CRxData[2 * i + 1] << 8) | CRxData[2 * i + 0]);	// CAN-Daten hinter ID extrahieren
        }

        switch (id)													// ID und CAN-Daten "sortieren"
                {
					case 0x46:
						canTimeoutCounters[0] = 0;					// Timeoutzähler zurücksetzen
						idReceived[0] = true;						// ID als empfangen markieren
						modelListener->CANIntReceived(id, intData);	// Empfangene CAN-Daten übertragen
					break;
					case 0x47:
						canTimeoutCounters[1] = 0;
						idReceived[1] = true;
						modelListener->CANIntReceived(id, intData);
						break;
                    case 0x50:
                        canTimeoutCounters[2] = 0;
                        idReceived[2] = true;
                        modelListener->CANIntReceived(id, intData);
                        break;
                    case 0x51:
                        canTimeoutCounters[3] = 0;
                        idReceived[3] = true;
                        modelListener->CANIntReceived(id, intData);
                        break;
                }
	  }


    for (int i = 0; i < 4; i++)			// Zähler hochzählen, wenn für bestimmte CAN-IDs keine Daten empfangen wurden
    {
        if (!idReceived[i])
        {
            canTimeoutCounters[i]++;	// Zähler für nicht empfangene IDs hochzählen

            if (canTimeoutCounters[i] > CAN_TIMEOUT_THRESHOLD)
            {
                canTimeoutCounters[i] = CAN_TIMEOUT_THRESHOLD;
            }
        }
    }


    for (int i = 0; i < 4; i++)				//CAN-ID die Timeout?
    {
        if (canTimeoutCounters[i] >= CAN_TIMEOUT_THRESHOLD)
        {
            for (int j = 0; j < 4; j++)
            {
                intData[j] = 0;				// 0 Senden wenn keine Daten von ID kommen
            }

            switch (i) {
                case 0:
                    modelListener->CANIntReceived(0x46, intData);
                    break;
                case 1:
                    modelListener->CANIntReceived(0x47, intData);
                    break;
                case 2:
                    modelListener->CANIntReceived(0x50, intData);
                    break;
                case 3:
                    modelListener->CANIntReceived(0x51, intData);
                    break;
            }
        }
    }

}


void Model::sendLinDataOverCAN()
{
	extern FDCAN_HandleTypeDef hfdcan1;
    uint8_t hvVoltage = tempRxData[16]; 					// HV Ist-Spannung
    uint8_t hvCurrent = tempRxData[15]; 					// HV Ist-Strom
    uint8_t canMessage[8]; 									// Standard-CAN-Frames zum Senden (8 Datenbytes)
    canMessage[0] = hvVoltage;
    canMessage[1] = hvCurrent;
    uint32_t canID = 0x123; 								// CAN Sende-ID
    EDT_FDCANx_Send_Msg(&hfdcan1, canID, canMessage, 2);	// LIN-Daten über CAN senden
}


void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	lastSize = Size;
    HAL_UARTEx_ReceiveToIdle_IT(&huart6, RxData, 22);

}


void Model::processLINData(uint8_t* data, uint16_t size)
{
    if (size == 12)
    {
    	rearrangeRxData(data, size);
    	modelListener->onLINStatusReceived(data);
        linTimeoutCounter = 0;
    }
    else
    {
        linTimeoutCounter++;
    }
}


void Model::rearrangeRxData(uint8_t* data, uint8_t size)
{
    if (size != 22) return;			// Größe korrekt?

    uint8_t temp[12];				// Daten in Temp Variable kopieren
    for (int i = 0; i < 12; i++)
    {
        temp[i] = data[i];
    }

    for (int i = 0; i < 10; i++)	// Verschieben der letzten 10 Bytes an den Anfang
    {
        data[i] = data[i + 12];
    }

    for (int i = 0; i < 12; i++)	// Zurückschieben der ersten 12 Bytes um 10 Positionen
    {
        data[i + 10] = temp[i];
    }
}


void Model::sendLINControlFrame(uint8_t hv_on, uint8_t regulator, uint8_t current, uint8_t voltage, uint8_t blower)
{
    uint8_t TxData[9];

    TxData[0] = 0x55;			// LIN Sync byte
    TxData[1] = pid_Calc(0x0A); // PID
    TxData[2] = (hv_on & 0x01) | ((regulator & 0x03) << 1);
    TxData[3] = 0;
    TxData[4] = current;
    TxData[5] = voltage;
    TxData[6] = (blower & 0x0F) << 4;
    TxData[7] = 0;
    TxData[8] = checksum_Calc(TxData[1], &TxData[2], 6);		// Checksum erzeugen

    HAL_LIN_SendBreak(&huart6);									// Sendet LIN Break
    HAL_UART_Transmit(&huart6, TxData, sizeof(TxData), 1000);	// Daten an LIN-Transceiver senden
}


void Model::receiveLINStatusFrame()
{
    uint8_t TxData[2] = {0x55, pid_Calc(0x10)}; // Sync Byte and PID for receiving status
    HAL_LIN_SendBreak(&huart6); 				// Sendet LIN Break
    HAL_UART_Transmit(&huart6, TxData, 2, 100); // Sendet Sync und PID
}


uint8_t Model::receiveInitialHviValue()
{
    receiveLINStatusFrame(); // sendet  Anfrage und ruft  Status ab
    return tempRxData[15];	// Status zurückgeben
}



/*
uint8_t Model::getHviValue()
{
    receiveLINStatusFrame(); // sendet  Anfrage und ruft Status ab
    return tempRxData[15];	// Status zurückgeben
}
*/


uint8_t Model::pid_Calc(uint8_t ID)
{
    if (ID > 0x3F)
    {
        Error_Handler();
    }

    uint8_t P0 = (ID ^ (ID >> 1) ^ (ID >> 2) ^ (ID >> 4)) & 0x01;
    uint8_t P1 = ~((ID >> 1) ^ (ID >> 3) ^ (ID >> 4) ^ (ID >> 5)) & 0x01;

    return ID | (P0 << 6) | (P1 << 7);// Rückgabe des vollständigen PID
}


uint8_t Model::checksum_Calc(uint8_t PID, uint8_t *data, uint8_t size)	// LIN checksum erzeugen
 {
     uint16_t sum = PID; // Beginnen mit dem Identifier

     for (uint8_t i = 0; i < size; i++)
     {
         sum += data[i];
         if (sum > 256)
         {
           sum -= 255;
         }
     }
   return ~sum & 0xFF;
 }


    bool Model::validateLINData(uint8_t *data)
    {
        uint8_t hwVersion = data[20] & 0x0F;  // Hardware-Version (Bits 0-3)
        uint8_t swVersion = (data[20] >> 4) & 0x0F;  // Software-Version (Bits 4-7)

        if (hwVersion > 0 && swVersion > 0) {
            return true;  // Gültige Daten
        }

        return false;  // Ungültige Daten, da HW oder SW gleich 0 ist
    }

