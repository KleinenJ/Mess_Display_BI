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

#define B_SAMPLE

// Globale Variablen für SD
//FATFS fs;    // Dateisystem
//FIL file;    // Dateiobjekt
//UINT bytesWritten;
//char buffer[100];
//FRESULT res;
//uint8_t wtext[] = "Test \n";
bool state_SDCS = false;

/// 2025/05/06, JKL: added system image
int16_t iTempVFmV = 0;
int16_t iHumidVFmV = 0;
int16_t iTempNFmV = 0;
int16_t iHumidNFmV = 0;

float fTempVF = 25.0f;
float fTempNF = 25.0f;
float fHumidVF = 50.0f;
float fHumidNF = 50.0f;

float fIonVoltage  = 0.0f;
float fIonCurrent  = 0.0f;
float fPolVoltage  = 0.0f;

float fPvF10  = 0.0f;
float fPvF25  = 0.0f;
float fPnF10  = 0.0f;
float fPnF25  = 0.0f;

float fLinIonVoltage  = 0.0f;
float fLinIonCurrent  = 0.0f;
float fLinIonSetCurrent  = 0.0f;

uint8_t bLinType = 255; 	// 0:= A-Sample
							// 1:= B-Sample
							// ...
							// 255:= unknown

uint8_t bLinVersion = 255;	// 255:= unknown

uint8_t bSubImageSelect = 0; /// die vier fragmente des Images müssen nacheinander versendet werden, für das Umschalten wird diese Variable verwendet


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
uint8_t uiaCanRxBuffer[8];	// global buffer for
uint8_t uiaCanTxBuffer[8];	// global buffer for


//uint32_t Rx_Mem[1];
//uint32_t Tx_Mem[] = {0x28};
uint8_t tickCounter = 0;

//#define SD_CS_GPIO_Port GPIOA  // Port A für den CS-Pin (PA15)
//#define SD_CS_Pin GPIO_PIN_15  // Pin 15 als CS-Pin

Model* modelInstance = nullptr; // Globale Instanz von Model


Model::Model() : modelListener(0)//, hviValue(0)
{

	modelInstance = this;


	this->activateHV(); // initial activation of HV

	// request Data from A-Sample
//	#ifdef B_SAMPLE
//    	uint8_t TxData[2] = {0x55, pid_Calc(0x09)}; // LIN-PID 0x09 for B-Samples
//	#else
//    	uint8_t TxData[2] = {0x55, pid_Calc(0x10)}; // LIN-PID 0x10 for A-Samples
//	#endif
//
//    HAL_LIN_SendBreak(&huart6); 				// LIN Break senden
//    HAL_UART_Transmit(&huart6, TxData, 2, 100); // Sync Byte und PID für Statusabfrage senden
    // check if A-Sample answered
    // if yes, set device type to "A"
    bLinType = 0;
    // if else request Data fom B-Sample

    // check if B-Sample answered

    // if yes, set device type to "B"
    // bLinType = 1;
    // else set device type to "X"
    // bLinType = 255;
}

void Model::tick()
{
	process_CAN_messages();

    linTimeoutCounter++;		 // LIN-Timeout-Zähler hochzählen

	const uint32_t tickThreshold = 75;
	tickCounter++;

	    if (tickCounter >= tickThreshold)	        	// Anfrage an den HV-Slave nach 75 Ticks senden
	    {
			#ifdef B_SAMPLE
	    		uint8_t TxData[2] = {0x55, pid_Calc(0x09)}; // LIN-PID 0x09 --> B-Muster
	        #else
	    		uint8_t TxData[2] = {0x55, pid_Calc(0x10)}; // LIN-PID 0x10 --> A-Muster
			#endif
	    	HAL_LIN_SendBreak(&huart6); 				// LIN Break senden
	        HAL_UART_Transmit(&huart6, TxData, 2, 100); // Sync Byte und PID für Statusabfrage senden
//
//	        uint8_t tmp_rx[12];
//	        HAL_UART_Receive(&huart6, tmp_rx, 12, 10); // 10ms Timeout

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

		#ifdef B_SAMPLE
			fLinIonVoltage = (-1.0) * (tempRxData[6] + (tempRxData[7]&&0x3) * 256); 				// HV Ist-Spannung = Rohwert *40
			fLinIonCurrent = (-1.0) * tempRxData[1] / 2.0f; 			// HV Ist-Strom = Rohwert / 2
		#else
	    	fLinIonVoltage = (-1.0) * tempRxData[16] * 40; 				// HV Ist-Spannung = Rohwert *40
			fLinIonCurrent = (-1.0) * tempRxData[15] / 2.0f; 			// HV Ist-Strom = Rohwert / 2
		#endif


		sendSystemImageOverCAN();


	   	state_SDCS = getSDCS();
	   	modelListener->setStateSDCS(state_SDCS);

}


int Model::ReadValueFromEEPORM()
{
	int iReadValue = EE_Read_PWM();

    fLinIonSetCurrent = (-1.0) * (iReadValue / 2.0f);
	return iReadValue;
}


void Model::SendVlaueToEEPROM(int value)
{
  EE_Write_PWM(value);
}

void Model::updatefLinIonSetCurrent(int value)
{
    fLinIonSetCurrent = (-1.0) * (value / 2.0f);
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
		  //char myFilename[] = "Text1.txt";
		  //char myData = value;
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

	float floatData[4];

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
				//modelListener->CANIntReceived(id, intData);	// Empfangene CAN-Daten übertragen

				iTempVFmV = intData[0]*10;	// Extract raw value
				iHumidVFmV = intData[1]*10;	// Extract raw value
				iTempNFmV = intData[2]*10;	// Extract raw value
				iHumidNFmV = intData[3]*10;	// Extract raw value


				if(iTempVFmV < 715) fTempVF = 81.0f;
				else if (iTempVFmV <= 911) fTempVF= ((iTempVFmV - 715) * (-0.0510204081632653)) + 80.0f;
				else if (iTempVFmV <= 1158) fTempVF= ((iTempVFmV - 911) * (-0.0404858299595142)) + 70.0f;
				else if (iTempVFmV <= 1469) fTempVF= ((iTempVFmV - 1158) * (-0.0321543408360129)) + 60.0f;
				else if (iTempVFmV <= 1842) fTempVF= ((iTempVFmV - 1469) * (-0.0268096514745308)) + 50.0f;
				else if (iTempVFmV <= 2270) fTempVF= ((iTempVFmV - 1842) * (-0.0233644859813084)) + 40.0f;
				else if (iTempVFmV <= 2500) fTempVF= ((iTempVFmV - 2270) * (-0.0217391304347826)) + 30.0f;
				else if (iTempVFmV <= 2736) fTempVF= ((iTempVFmV - 2500) * (-0.0211864406779661)) + 25.0f;
				else if (iTempVFmV <= 3210) fTempVF= ((iTempVFmV - 2736) * (-0.0210970464135021)) + 20.0f;
				else if (iTempVFmV <= 3657) fTempVF= ((iTempVFmV - 3210) * (-0.0223713646532438)) + 10.0f;
				else if (iTempVFmV <= 4048) fTempVF= ((iTempVFmV - 3657) * (-0.0255754475703325)) + 0.0f;
				else if (iTempVFmV <= 4361) fTempVF= ((iTempVFmV - 4048) * (-0.0319488817891374)) - 10.0f;
				else if (iTempVFmV <= 4595) fTempVF= ((iTempVFmV - 4361) * (-0.0427350427350427)) - 20.0f;
				else if (iTempVFmV <= 4757) fTempVF= ((iTempVFmV - 4595) * (-0.0617283950617284)) - 30.0f;
				else if (iTempVFmV > 4757) fTempVF= -41.0f;

				if(iHumidVFmV < 1235) fHumidVF = 9.0f;
				else if (iHumidVFmV <= 3555) fHumidVF = (iHumidVFmV * 0.0366379310344828f) - 35.2478448f;
				else fHumidVF = 96.0f;

				if(iTempNFmV < 715) fTempNF = 81.0f;
				else if (iTempNFmV <= 911) fTempNF= ((iTempNFmV - 715) * (-0.0510204081632653)) + 80.0f;
				else if (iTempNFmV <= 1158) fTempNF= ((iTempNFmV - 911) * (-0.0404858299595142)) + 70.0f;
				else if (iTempNFmV <= 1469) fTempNF= ((iTempNFmV - 1158) * (-0.0321543408360129)) + 60.0f;
				else if (iTempNFmV <= 1842) fTempNF= ((iTempNFmV - 1469) * (-0.0268096514745308)) + 50.0f;
				else if (iTempNFmV <= 2270) fTempNF= ((iTempNFmV - 1842) * (-0.0233644859813084)) + 40.0f;
				else if (iTempNFmV <= 2500) fTempNF= ((iTempNFmV - 2270) * (-0.0217391304347826)) + 30.0f;
				else if (iTempNFmV <= 2736) fTempNF= ((iTempNFmV - 2500) * (-0.0211864406779661)) + 25.0f;
				else if (iTempNFmV <= 3210) fTempNF= ((iTempNFmV - 2736) * (-0.0210970464135021)) + 20.0f;
				else if (iTempNFmV <= 3657) fTempNF= ((iTempNFmV - 3210) * (-0.0223713646532438)) + 10.0f;
				else if (iTempNFmV <= 4048) fTempNF= ((iTempNFmV - 3657) * (-0.0255754475703325)) + 0.0f;
				else if (iTempNFmV <= 4361) fTempNF= ((iTempNFmV - 4048) * (-0.0319488817891374)) - 10.0f;
				else if (iTempNFmV <= 4595) fTempNF= ((iTempNFmV - 4361) * (-0.0427350427350427)) - 20.0f;
				else if (iTempNFmV <= 4757) fTempNF= ((iTempNFmV - 4595) * (-0.0617283950617284)) - 30.0f;
				else if (iTempNFmV > 4757) fTempNF= -41.0f;

				if(iHumidNFmV < 1235) fHumidNF = 9.0f;
				else if (iHumidNFmV <= 3555) fHumidNF = (iHumidNFmV * 0.0366379310344828f) - 35.2478448f;
				else fHumidNF = 96.0f;

				floatData[0]= fTempVF;
				floatData[1]= fHumidVF;
				floatData[2]= fTempNF;
				floatData[3]= fHumidNF;
				/// add new transfer function

				modelListener->CANFloatReceived(id, floatData);	// Empfangene CAN-Daten übertragen
				break;

			case 0x47:
				canTimeoutCounters[1] = 0;
				idReceived[1] = true;
				//modelListener->CANIntReceived(id, intData);

				fPvF10 = static_cast<float>(intData[0]) * 1.003f;	// 1.003009027081244 - Umrechnungsfaktor wegen 500 Ohm bei PM (0 - 20mA)
				fPvF25 = static_cast<float>(intData[1]) * 1.003f;
				fPnF10 = static_cast<float>(intData[2]) * 1.003f;
				fPnF25 = static_cast<float>(intData[3]) * 1.003f;

				if(fPvF10 > 1000) fPvF10 = 1000;
				if(fPvF25 > 1000) fPvF25 = 1000;
				if(fPnF10 > 1000) fPnF10 = 1000;
				if(fPnF25 > 1000) fPnF25 = 1000;
				if(fPvF10 < 0) fPvF10 = 0;
				if(fPvF25 < 0) fPvF25 = 0;
				if(fPnF10 < 0) fPnF10 = 0;
				if(fPnF25 < 0) fPnF25 = 0;

				floatData[0]= fPvF10;
				floatData[1]= fPvF25;
				floatData[2]= fPnF10;
				floatData[3]= fPnF25;

				modelListener->CANFloatReceived(id, floatData);	// Empfangene CAN-Daten übertragen
				break;

			case 0x50:
				canTimeoutCounters[2] = 0;
				idReceived[2] = true;

				//modelListener->CANIntReceived(id, intData);
				fIonVoltage = (static_cast<float>(intData[0]) +3.0)*(-8.0f);
				fPolVoltage = (static_cast<float>(intData[1]) +4.0)*(-3.0f);
				//fIonCurrent = (static_cast<float>(intData[2]) / 10.0f) * (-1.25f);
				fIonCurrent = (static_cast<float>(intData[2]) / 10.0f) * (-0.8789f);

				if(fIonVoltage > 0) fIonVoltage = 0;
				if(fIonVoltage < -8000) fIonVoltage = -8000;

				if(fPolVoltage > 0) fPolVoltage = 0;
				if(fPolVoltage < -3000) fPolVoltage = -3000;

				if(fIonCurrent > 0) fIonCurrent = 0;
				if(fIonCurrent < -100) fIonCurrent = -100;

				floatData[0]= fIonVoltage;
				floatData[1]= fPolVoltage;
				floatData[2]= fIonCurrent;
				floatData[3]= 0.0f;			/// default

				modelListener->CANFloatReceived(id, floatData);	// Empfangene CAN-Daten übertragen
				break;

			case 0x51:
				canTimeoutCounters[3] = 0;
				idReceived[3] = true;
				// discard ,essage content
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

	uint8_t canMessage[2];
	canMessage[0] = tempRxData[16];
	canMessage[1] = tempRxData[15];

    uint32_t canID = 0x123; 								// CAN Sende-ID
    EDT_FDCANx_Send_Msg(&hfdcan1, canID, canMessage, 2);	// LIN-Daten über CAN senden

}

void Model::sendSystemImageOverCAN()
{
	/// das gesamte Image braucht drei CAN-Nachrichten
	/// die ID's der Statusnachrichten, sind extra größer als die der Messnachrichten, um die Messwertübertragung höher zu prorisieren.

/*	this->fnSendFloatsViaCAN(0x005a, ((fTempVF * 10.0f)), (fHumidVF * 10.0f), (fPvF10 * 10.0f), (fPvF25 * 10.0f));
	this->fnSendFloatsViaCAN(0x005b, fIonVoltage, (fIonCurrent * 10.0f), fPolVoltage, 0.0f);
	this->fnSendFloatsViaCAN(0x005c, fLinIonVoltage, (fLinIonCurrent * 10.0f), (fLinIonSetCurrent * 10.0f), 0.0f);
	this->fnSendFloatsViaCAN(0x005d, ((fTempNF * 10.0f)), (fHumidNF * 10.0f), (fPnF10 * 10.0f), (fPnF25 * 10.0f));
*/

	switch(bSubImageSelect)
	{
		case 0:	/// 1. (0x005a bzw. 90|dec) Luftwerte vor dem Filter
				/// [0-1] fTempVF
				/// [2-3] fHumidVF
				///	[4-5] fPvF10
				/// [6-7] fPvF25
				//uiaCanRxBuffer


				this->fnSendFloatsViaCAN (0x005a, ((fTempVF * 10.0f)), (fHumidVF * 10.0f), (fPvF10 * 10.0f), (fPvF25 * 10.0f));
				bSubImageSelect = 1;
				break;

		case 1:	// 2. (0x005b bzw. 91|dec) Elektrische Werte des Filters
				/// [0-1] fIonVoltage
				/// [2-3] fIonCurrent
				///	[4-5] fPolVoltage
				/// [6-7] Reserve
				this->fnSendFloatsViaCAN(0x005b, fIonVoltage, (fIonCurrent * 10.0f), fPolVoltage, 0.0f);
				bSubImageSelect = 2;
				break;

		case 2:	// 3. (0x005c bzw. 92|dec)LIN-Werte der HV-Quelle
				/// [0-1] 	fLinIonVoltage
				/// [2-3] 	fLinIonCurrent
				///	[4-5] 	fLinIonSetCurrent
				/// [6-7] Reserve
				this->fnSendFloatsViaCAN(0x005c, fLinIonVoltage, (fLinIonCurrent * 10.0f), (fLinIonSetCurrent * 10.0f), 0.0f);

				bSubImageSelect = 3;
				break;

		case 3:	// 4. (0x005d bzw. 93|dec)Luftwerte nach dem Filter
				/// [0-1] fTempNF
				/// [2-3] fHumidNF
				///	[4-5] fPnF10
				/// [6-7] fPnF25
				this->fnSendFloatsViaCAN(0x005d, ((fTempNF * 10.0f)), (fHumidNF * 10.0f), (fPnF10 * 10.0f), (fPnF25 * 10.0f));

				bSubImageSelect = 0;
				break;

		default:
				bSubImageSelect = 0;
				break;
	}
}

void Model::fnSendFloatsViaCAN(uint32_t uiCanID, float fValue1, float fValue2, float fValue3, float fValue4)
{
	extern FDCAN_HandleTypeDef hfdcan1;

	int16_t baInputValues[4];//={0xffff,0xffff,0xffff,0xffff};
	uint8_t baMessageBuffer[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};

	baInputValues[0]= (int16_t) (fValue1);
	baInputValues[1]= (int16_t) (fValue2);
	baInputValues[2]= (int16_t) (fValue3);
	baInputValues[3]= (int16_t) (fValue4);

	baMessageBuffer[0]=  baInputValues[0] & 0xFF;
	baMessageBuffer[1]= (baInputValues[0] >> 8)  & 0xFF;
	baMessageBuffer[2]=  baInputValues[1] & 0xFF;
	baMessageBuffer[3]= (baInputValues[1] >> 8)  & 0xFF;
	baMessageBuffer[4]=  baInputValues[2] & 0xFF;
	baMessageBuffer[5]= (baInputValues[2] >> 8)  & 0xFF;
	baMessageBuffer[6]=  baInputValues[3] & 0xFF;
	baMessageBuffer[7]= (baInputValues[3] >> 8)  & 0xFF;

    EDT_FDCANx_Send_Msg(&hfdcan1, uiCanID, baMessageBuffer, 8);	// LIN-Daten über CAN senden
}

void Model::fnTxViaCAN(uint32_t uiCanID, uint8_t bLength, uint8_t *baBuffer)
{
	extern FDCAN_HandleTypeDef hfdcan1;

    EDT_FDCANx_Send_Msg(&hfdcan1, uiCanID, baBuffer, bLength);	// LIN-Daten über CAN senden
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
    uint8_t TxData[11];

	#ifdef B_SAMPLE
		if (current > 60)current= 60; // limit current settings

    	uint8_t uiSetCurrent = 255 - (19/2.5) * (current/2.0) - (2 * current/60); // (2 * current/60) is to compensate a nonlinearity which effects the result at higher currents

		TxData[0] = 0x55;			// LIN Sync byte
		TxData[1] = pid_Calc(0x20); // PID for B-Sample Labor-Frame
		TxData[2] = 0x02;			// Select Labor-Frame Level 2
		TxData[3] = uiSetCurrent;
		TxData[4] = 0xff;
		TxData[5] = 0xff;
		TxData[6] = 0xff;
		TxData[7] = 0xff;
		TxData[8] = 0xff;
		TxData[9] = 0xff;
		TxData[10] = checksum_Calc(TxData[1], &TxData[2], 8);
	#else
		TxData[0] = 0x55;			// LIN Sync byte
		TxData[1] = pid_Calc(0x0A); // PID
		TxData[2] = (hv_on & 0x01) | ((regulator & 0x03) << 1);
		TxData[3] = 0;
		TxData[4] = current;
		TxData[5] = voltage;
		TxData[6] = (blower & 0x0F) << 4;
		TxData[7] = 0;
		TxData[8] = checksum_Calc(TxData[1], &TxData[2], 6);		// Checksum erzeugen
	#endif

	HAL_LIN_SendBreak(&huart6);									// Sendet LIN Break
    HAL_UART_Transmit(&huart6, TxData, sizeof(TxData), 1000);	// Daten an LIN-Transceiver senden
}

void Model::activateHV()
{
    uint8_t TxData[11];

	#ifdef B_SAMPLE
		TxData[0] = 0x55;			// LIN Sync byte
		TxData[1] = pid_Calc(0x06); // PID PCUe Frame
		TxData[2] = 0x1e;			// 0x1? turns HV on
		TxData[3] = 0x00;
		TxData[4] = 0xfe;
		TxData[5] = 0xfe;
		TxData[6] = 0x00;
		TxData[7] = 0xfe;
		TxData[8] = 0xc3;
		TxData[9] = 0xff;
		TxData[10] = checksum_Calc(TxData[1], &TxData[2], 8);
	#else
		// t.b.d.
	#endif

	HAL_LIN_SendBreak(&huart6);									// Sendet LIN Break
    HAL_UART_Transmit(&huart6, TxData, sizeof(TxData), 1000);	// Daten an LIN-Transceiver senden
}

void Model::disableHV()
{
    uint8_t TxData[11];

	#ifdef B_SAMPLE
		TxData[0] = 0x55;			// LIN Sync byte
		TxData[1] = pid_Calc(0x06); // PID PCUe Frame
		TxData[2] = 0x0e;			// 0x0? turns HV off
		TxData[3] = 0x00;
		TxData[4] = 0xfe;
		TxData[5] = 0xfe;
		TxData[6] = 0x00;
		TxData[7] = 0xfe;
		TxData[8] = 0xc3;
		TxData[9] = 0xff;
		TxData[10] = checksum_Calc(TxData[1], &TxData[2], 8);
	#else
		// t.b.d.
	#endif

	HAL_LIN_SendBreak(&huart6);									// Sendet LIN Break
    HAL_UART_Transmit(&huart6, TxData, sizeof(TxData), 1000);	// Daten an LIN-Transceiver senden
}


void Model::receiveLINStatusFrame()
{
	#ifdef B_SAMPLE
    	uint8_t TxData[2] = {0x55, pid_Calc(0x09)}; // Sync Byte and PID for receiving status from B-Samples (0x09)
	#else
    	uint8_t TxData[2] = {0x55, pid_Calc(0x10)}; // Sync Byte and PID for receiving status from A-Samples (0x10)
	#endif

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

