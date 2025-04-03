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
			/// JKL, 2025/04/03: changed static temperature calculation to "LookUpTable"
			float fTempVF = 0.0;
			int16_t iTempVFmV = value1;	// Extract raw value

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

			/// JKL, 2025/04/03: changed static temperature calculation to "LookUpTable"
			float fTempNF = 0.0;
			int16_t iTempNFmV = value3;	// Extract raw value

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

			/// JK, 2025/04/03: Umrechnen der Feuchtigkeitswerte
			float fHumidVF = 0.0f;
			int16_t iHumidVFmV = value2;	// Extract raw value

			if(iHumidVFmV < 1235) fHumidVF = 9.0f;
			else if (iHumidVFmV <= 3555) fHumidVF = (iHumidVFmV * 0.0366379310344828f) - 35.2478448f;
			else fHumidVF = 96.0f;

			float fHumidNF = 0.0f;
			int16_t iHumidNFmV = value4;	// Extract raw value

			if(iHumidNFmV < 1235) fHumidNF = 9.0f;
			else if (iHumidNFmV <= 3555) fHumidNF = (iHumidNFmV * 0.0366379310344828f) - 35.2478448f;
			else fHumidNF = 96.0f;


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
