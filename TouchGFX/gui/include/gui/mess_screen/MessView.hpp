#ifndef MESSVIEW_HPP
#define MESSVIEW_HPP

#include <gui_generated/mess_screen/MessViewBase.hpp>
#include <gui/mess_screen/MessPresenter.hpp>

#include "fatfs.h"
#include <stdio.h>
#include <string.h>



class MessView : public MessViewBase
{
public:
    MessView();
    virtual ~MessView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void Adj_Displ_PWM(int value);
   // virtual void HVU_Plus();
  //  virtual void HVU_Minus();
    virtual void HVI_Plus();
    virtual void HVI_Minus();
    void CANIntReceived(uint32_t ID, const int16_t *data);
    // Method to display LIN status
    //void displayLINStatus(uint8_t hvStatus, uint8_t regulationType, uint8_t regulatorStatus, uint8_t linError, uint8_t currentActualValue, uint8_t voltageActualValue, uint8_t hardwareVersion, uint8_t softwareVersion);
    void updateLINStatus(uint8_t *status);

    //void updateHviValue(uint8_t *hviValue);

    //void getEEPROMValue();
    //void EEPROMRead();
    //void getEEPROMCount();
    //virtual void EEPROMWrite();

    void text_write();
    void setStateSDCS(bool state_SDCS_model);

private:
    bool hwVersionInitialized;
    bool swVersionInitialized;
    float hvi;
    uint8_t receivedVoltageValue;
    int ReadEEPROMValue;
    //int EEcount;

protected:
};

#endif // MESSVIEW_HPP
