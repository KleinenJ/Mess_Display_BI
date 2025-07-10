#include <gui/mess_screen/MessView.hpp>

int updateCounter = 0;
int UPDATE_TIMER_THRESHOLD = 50;

MessView::MessView() : hwVersionInitialized(false), swVersionInitialized(false), receivedVoltageValue(110)
{

}

void MessView::setupScreen()
{
    MessViewBase::setupScreen();
    updateCounter = 0;
    presenter->setBacklightValuePresenter(10);

    presenter->sendLINControlFrame(1, 1, 0, 0, 5);
    presenter->sendLINControlFrame(0, 0, 0, 0, 5);
    HAL_Delay(100);

    hvi = 60;							// Temp Startwert bis Flash gelesen wird
    hvi = presenter->readEEPROMvalue();	// liest letzten HV-Wert Sollstrom

    presenter->sendLINControlFrame(1, 1, hvi, 55, 8);
    presenter->receiveLINStatusFrame();

    Unicode::snprintfFloat(HVIBuffer, HVI_SIZE, "%2.1f", hvi / 2.0f);

    HVI.invalidate();

    text_write();
}

void MessView::tearDownScreen()
{
    MessViewBase::tearDownScreen();
}

/*
void MessView::getEEPROMValue()
{
  ReadEEPROMValue = presenter->readEEPROMvalue();
}

void MessView::EEPROMWrite()
{
presenter->sendEEPROMvalue(19);
EEPROMRead();
}

void MessView::EEPROMRead()
{
      getEEPROMValue();
}
*/

void MessView::updateIonVoltage(float value)
{
    presenter->setBacklightValuePresenter(value);
}

void MessView::Adj_Displ_PWM(int value)
{
    presenter->setBacklightValuePresenter(value);
}


void MessView::setStateSDCS(bool state_SDCS_model)
{
  if (state_SDCS_model == true)
  {
  }
  else if (state_SDCS_model == false)
  {
  }
}


void MessView::text_write()
{
	  FATFS SDFatFS; 	//Fatfs handle
	  FIL SDFile; 		//File handle
	  FRESULT fresult; //Result after operations
	  char myFilename[] = "Text1.txt";
	  char myData[] = "Hello World";
	//uint8_t wtext[] = "This is working with FatFs + SPI \n";
	  UINT myBytes;
	//BYTE readBuf[30];

	fresult = f_mount(&SDFatFS, "/", 1); //1=mount now
	  if (fresult != FR_OK)
	  {

	  }
	  fresult = f_open(&SDFile, myFilename, FA_WRITE || FA_CREATE_ALWAYS);
	    if(fresult == FR_OK)
	    {

	    }

	    //strncpy((char*)readBuf, "a new file is made!", 18);
	    //readBuf[18] = '\0';
	    //UINT bytesWrote;
	    fresult = f_write(&SDFile, myData, sizeof(myData), &myBytes);
	     if(fresult == FR_OK)
	     {

	     }

	     f_close(&SDFile);
	     f_mount(NULL, "/", 1);
}


void MessView::HVI_Plus()
{
	hvi = hvi + 1;
	if (hvi >= 60) hvi = 60;		// Stromgrenze

    presenter->sendLINControlFrame(1,1,hvi,receivedVoltageValue,8);

    presenter->setHviValue(hvi);	// Wert an den Presenter weitergeben
    presenter->receiveLINStatusFrame();

    updateCounter = 0;  // Zähler zurücksetzen

    presenter->sendEEPROMvalue(hvi);

}
void MessView::HVI_Minus()
{
	hvi = hvi - 1;
	if (hvi < 10)	hvi = 10; // minimaler Stromwert 5µA

    presenter->sendLINControlFrame(1,1,hvi,receivedVoltageValue,8);

    presenter->setHviValue(hvi);  // Wert an den Presenter weitergeben
    presenter->receiveLINStatusFrame();

    updateCounter = 0;  // Zähler zurücksetzen

    presenter->sendEEPROMvalue(hvi);
}


void MessView::CANFloatReceived(uint32_t ID, const float *data)
{
	if (ID == 0x46)
	    {
			/// JKL, 2025/04/03: changed static temperature calculation to "LookUpTable"
			float fTempVF = data[0];
			float fTempNF = data[2];
			float fHumidVF = data[1];
			float fHumidNF = data[3];

			Unicode::snprintfFloat(TvFBuffer, TVF_SIZE, "%2.1f", fTempVF);
			Unicode::snprintfFloat(LvFBuffer, LVF_SIZE, "%2.0f", fHumidVF);
			Unicode::snprintfFloat(TnFBuffer, TNF_SIZE, "%2.1f", fTempNF);
			Unicode::snprintfFloat(LnFBuffer, LNF_SIZE, "%2.0f", fHumidNF);

			TvF.invalidate();
			LvF.invalidate();
			TnF.invalidate();
			LnF.invalidate();
	    }

	if (ID == 0x47)
	    {
			float fPvF10 = data[0];
			float fPvF25 = data[1];
			float fPnF10 = data[2];
			float fPnF25 = data[3];

			Unicode::snprintfFloat(PvF10Buffer, PVF10_SIZE, "%1.0f", fPvF10);
			Unicode::snprintfFloat(PvF25Buffer, PVF25_SIZE, "%1.0f", fPvF25);
			Unicode::snprintfFloat(PnF10Buffer, PNF10_SIZE, "%1.0f", fPnF10);
			Unicode::snprintfFloat(PnF25Buffer, PNF25_SIZE, "%1.0f", fPnF25);

			PvF10.invalidate();
			PvF25.invalidate();
			PnF10.invalidate();
			PnF25.invalidate();
	    }

	if (ID == 0x50)
	    {

			float fIonVoltage = data[0];
			float fPolVoltage = data[1];
			float fIonCurrent = data[2];

			Unicode::snprintfFloat(MIoUBuffer, MIOU_SIZE, "%3.0f", fIonVoltage);	// Anzeige mit 1 Nachkommastelle
			Unicode::snprintfFloat(MPoUBuffer, MPOU_SIZE, "%3.0f", fPolVoltage);	// Anzeige mit 1 Nachkommastelle
			Unicode::snprintfFloat(MPoABuffer, MPOA_SIZE, "%3.1f", fIonCurrent);	// Anzeige mit 1 Nachkommastelle

			MIoU.invalidate();
			MPoU.invalidate();
			MPoA.invalidate();
	    }

	if (ID == 0x51)	// Muss nicht auf dem Display ausgegeben werden / Reserve
	    {
	            //Unicode::snprintf(Mess_HV_I_VBuffer, MESS_HV_I_V_SIZE, "%d", value1);
	            //Unicode::snprintf(Mess_HV_I_ABuffer, MESS_HV_I_A_SIZE, "%d", value2);
	            //Unicode::snprintf(Mess_HV_P_VBuffer, MESS_HV_P_V_SIZE, "%d", value3);
	            //Unicode::snprintf(zRESERVEBuffer, ZRESERVE_SIZE, "%d", value4);
	            //Mess_HV_I_V.invalidate();
	            //Mess_HV_I_A.invalidate();
	            //Mess_HV_P_V.invalidate();
	            //zRESERVE.invalidate();
	    }
}


void MessView::updateLINStatus(uint8_t *status)
{
    static uint8_t oldHVI = 0;
    static uint16_t oldHVU = 0;

    uint8_t newHVI = status[3];
    uint16_t newHVU = (((status[8]&0x3) << 8) + (status[7]));

    float fLinIonVoltage = (-1.0) * (((status[8]&0x3) << 8) + (status[7])); 				// HV Ist-Spannung = Rohwert *40
	float fLinIonCurrent = (-1.0) * status[3] / 2.0f; 			// HV Ist-Strom = Rohwert / 2

	receivedVoltageValue = newHVU;

    Unicode::snprintfFloat(HVUBuffer, HVU_SIZE, "%4.0f", fLinIonVoltage); // Display voltage in

    // float hviValue = (-1.0)*static_cast<float>(newHVI) / 2.0f;
    Unicode::snprintfFloat(HVIBuffer, HVI_SIZE, "%2.1f", fLinIonCurrent);

    float hvisollValue = (-1.0)*static_cast<float>(hvi) / 2.0f;
    Unicode::snprintfFloat(HVI_SollBuffer, HVI_SOLL_SIZE, "%2.1f", hvisollValue);


    if (newHVI != oldHVI || newHVU != oldHVU)	// Prüfe, ob sich der Wert von HVI oder HVU geändert hat
    {
        updateCounter = 0;
        oldHVI = newHVI;
        oldHVU = newHVU;


        if (newHVI == 0 && newHVU == 0) // Wenn neuer Wert 0, setze UPDATE_TIMER_THRESHOLD auf 5, sonst auf 50
        {								// Anpassung da es sonst zu lange dauert bis erkannt wird, das keine Daten kommen
            UPDATE_TIMER_THRESHOLD = 5;
        } else {
            UPDATE_TIMER_THRESHOLD = 50;
        }

    }


    uint8_t regulationState = status[2] & 0x03;	// Prüfen, welchen Status die HV-Quelle zurückgibt

        const char* statusText;
        switch (regulationState)
        {
            case 0:
                statusText = "Aus";
                presenter->sendLINControlFrame(1, 1, hvi, 55, 8);
                presenter->receiveLINStatusFrame();
				swVersionInitialized = false;
				hwVersionInitialized = false;
                break;
            case 1:
                statusText = "ein";
                break;
            case 2:
                statusText = "auto";
                break;
            case 3:
                statusText = "init";
                break;
            default:
                statusText = "Unbekannt";
                presenter->sendLINControlFrame(1, 1, hvi, 55, 8);
                presenter->receiveLINStatusFrame();
				swVersionInitialized = false;
				hwVersionInitialized = false;
                break;
        }

        Unicode::UnicodeChar unicodeStatusText[HV_STATUS_SIZE];				// Konvertiere statusText zu UnicodeChar
        Unicode::strncpy(unicodeStatusText, statusText, HV_STATUS_SIZE);	// Übergibt HV Status an das Display

        Unicode::snprintf(HV_StatusBuffer, HV_STATUS_SIZE, "%s", unicodeStatusText);

        Unicode::snprintf(HV_HWBuffer, HV_HW_SIZE, "x");

        Unicode::snprintf(HV_SWBuffer, HV_SW_SIZE, "x");

//        if (hwVersionInitialized == false && (status[20] & 0x0F) > 0 )		// HW/SW Version auslesen und einmalig auf Display setzen (sonst flackert es)
//        {
//                Unicode::snprintf(HV_HWBuffer, HV_HW_SIZE, "%d", status[20] & 0x0F);
//                hwVersionInitialized = true;  // Flag setzen, um die HW-Version nicht erneut zu setzen
//                HV_HW.invalidate();
//        }
//        if (swVersionInitialized == false && ((status[20] >> 4) & 0x0F) > 0)
//        {
//               Unicode::snprintf(HV_SWBuffer, HV_SW_SIZE, "%d", (status[20] >> 4) & 0x0F);
//               swVersionInitialized = true;  // Flag setzen, um die SW-Version nicht erneut zu setzen
//               HV_SW.invalidate();
//        }

    HV_HW.invalidate();
    HV_SW.invalidate();
    HVI_Soll.invalidate();

   if (!hwVersionInitialized)
   {
        HV_HW.invalidate();
   }
   if (!swVersionInitialized)
   {
        HV_SW.invalidate();
   }

   if (updateCounter >= UPDATE_TIMER_THRESHOLD)
    {
        HV_Status.invalidate();
        HVI.invalidate();
        HVU.invalidate();
        updateCounter = 0;  // Zähler zurücksetzen
    }
    else
    {
         updateCounter++;  // Zähler erhöhen
    }

}
