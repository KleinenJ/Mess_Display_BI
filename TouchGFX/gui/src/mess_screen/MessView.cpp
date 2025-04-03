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
	if (hvi >= 250) hvi = 250;		// Stromgrenze

    presenter->sendLINControlFrame(1,1,hvi,receivedVoltageValue,8);

    presenter->setHviValue(hvi);	// Wert an den Presenter weitergeben
    presenter->receiveLINStatusFrame();

    updateCounter = 0;  // Zähler zurücksetzen

    presenter->sendEEPROMvalue(hvi);

}
void MessView::HVI_Minus()
{
	if (hvi >= 1)	hvi = hvi - 1;

    if (hvi > 0)
    {
    presenter->sendLINControlFrame(1,1,hvi,receivedVoltageValue,8);
    }
    else
    {
    	presenter->sendLINControlFrame(0,1,hvi,0,8);
    }

    presenter->setHviValue(hvi);  // Wert an den Presenter weitergeben
    presenter->receiveLINStatusFrame();

    updateCounter = 0;  // Zähler zurücksetzen

    presenter->sendEEPROMvalue(hvi);
}


void MessView::CANIntReceived(uint32_t ID, const int16_t *data)
{
    int16_t value1 = data[0]; // 4 Empfangsdaten sortieren
    int16_t value2 = data[1];
    int16_t value3 = data[2];
    int16_t value4 = data[3];

	if (ID == 0x46)
	    {
				float temperature1 = static_cast<float>(value1) / 10.0f - 20.0f;  // Temp skalieren, mit Nachkommastelle und -20 Offset (DBC Formatierung)
				float temperature2 = static_cast<float>(value3) / 10.0f - 20.0f;

				if(temperature1 > 80) temperature1 = 80;		// Für Grenzen
				if(temperature2 > 80) temperature2 = 80;
				if(temperature1 < -20) temperature1 = -20;
				if(temperature2 < -20) temperature2 = -20;

				if(value2 > 1000) value2 = 1000;				// Für Grenzen
				if(value4 > 1000) value4 = 1000;
				if(value2 < 0) value2 = 0;
				if(value4 < 0) value4 = 0;

				//Unicode::snprintf(TvFBuffer, TVF_SIZE, "%d", (value1/10)-20);	// -20 bis + 80°C
				Unicode::snprintfFloat(TvFBuffer, TVF_SIZE, "%2.1f", temperature1);  // -20 bis + 80°C
				Unicode::snprintf(LvFBuffer, LVF_SIZE, "%d", value2/10);		// 0 bis 100%
				//Unicode::snprintf(TnFBuffer, TNF_SIZE, "%d", (value3/10)-20);	// -20 bis + 80°C
			    Unicode::snprintfFloat(TnFBuffer, TNF_SIZE, "%2.1f", temperature2);  // -20 bis + 80°C
				Unicode::snprintf(LnFBuffer, LNF_SIZE, "%d", value4/10);		// 0 bis 100%
				TvF.invalidate();
				LvF.invalidate();
				TnF.invalidate();
				LnF.invalidate();
	    }

	if (ID == 0x47)
	    {
				float pm1 = static_cast<float>(value1) * 1.003f;	// 1.003009027081244 - Umrechnungsfaktor wegen 500 Ohm bei PM (0 - 20mA)
				float pm2 = static_cast<float>(value2) * 1.003f;
				float pm3 = static_cast<float>(value3) * 1.003f;

				if(pm1 > 1000) pm1 = 1000;
				if(pm2 > 1000) pm2 = 1000;
				if(pm3 > 1000) pm3 = 1000;
				if(pm1 < 0) pm1 = 0;
				if(pm2 < 0) pm2 = 0;
				if(pm3 < 0) pm3 = 0;

				Unicode::snprintfFloat(PvFBuffer, PVF_SIZE, "%1.0f", pm1);
				//Unicode::snprintf(PvFBuffer, PVF_SIZE, "%d", value1);
	            //Unicode::snprintf(PnFBuffer, PNF_SIZE, "%d", pm2);
	            //Unicode::snprintf(PIBuffer, PI_SIZE, "%d", pm3);
				Unicode::snprintfFloat(PnFBuffer, PNF_SIZE, "%1.0f", pm2);
				Unicode::snprintfFloat(PIBuffer, PI_SIZE, "%1.0f", pm3);
	            //Unicode::snprintf(xRESERVEBuffer, XRESERVE_SIZE, "%d", value4);
	            PvF.invalidate();
	            PnF.invalidate();
	            PI.invalidate();
	            //xRESERVE.invalidate();
	    }

	if (ID == 0x50)
	    {
				float currentValue = (static_cast<float>(value3) / 10.0f) * 1.25f;		// Formatierung

				if(currentValue < 0) currentValue = 0;

	            Unicode::snprintf(MIoUBuffer, MIOU_SIZE, "%d", (value1 +3)*(-8));
	            Unicode::snprintf(MPoUBuffer, MPOU_SIZE, "%d", (value2 +4)*(-3));
	            Unicode::snprintfFloat(MPoABuffer, MPOA_SIZE, "%3.1f", currentValue);	// Anzeige mit 1 Nachkommastelle
	            //Unicode::snprintf(MPoABuffer, MPOA_SIZE, "%d", (value3*8);
	            //Unicode::snprintf(yRESERVEBuffer, YRESERVE_SIZE, "%d", value4);
	            MIoU.invalidate();
	            MPoU.invalidate();
	            MPoA.invalidate();
	            //yRESERVE.invalidate();
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
    static uint8_t oldHVU = 0;

    uint8_t newHVI = status[15];
    uint8_t newHVU = status[16];

	receivedVoltageValue = newHVU;

    Unicode::snprintf(HVUBuffer, HVU_SIZE, "%d", newHVU*40); // Display voltage in

    float hviValue = static_cast<float>(newHVI) / 2.0f;
    Unicode::snprintfFloat(HVIBuffer, HVI_SIZE, "%2.1f", hviValue);

    float hvisollValue = static_cast<float>(hvi) / 2.0f;
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


    uint8_t regulationState = (status[13] >> 4) & 0x03;	// Prüfen, welchen Status die HV-Quelle zurückgibt

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
                statusText = "Einregeln";
                break;
            case 2:
                statusText = "Eingeregelt";
                break;
            case 3:
                statusText = "Fehler";
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


        if (hwVersionInitialized == false && (status[20] & 0x0F) > 0 )		// HW/SW Version auslesen und einmalig auf Display setzen (sonst flackert es)
        {
                Unicode::snprintf(HV_HWBuffer, HV_HW_SIZE, "%d", status[20] & 0x0F);
                hwVersionInitialized = true;  // Flag setzen, um die HW-Version nicht erneut zu setzen
                HV_HW.invalidate();
        }
        if (swVersionInitialized == false && ((status[20] >> 4) & 0x0F) > 0)
        {
               Unicode::snprintf(HV_SWBuffer, HV_SW_SIZE, "%d", (status[20] >> 4) & 0x0F);
               swVersionInitialized = true;  // Flag setzen, um die SW-Version nicht erneut zu setzen
               HV_SW.invalidate();
        }

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
