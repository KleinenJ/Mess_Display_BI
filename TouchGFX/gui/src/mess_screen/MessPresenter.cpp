#include <gui/mess_screen/MessView.hpp>
#include <gui/mess_screen/MessPresenter.hpp>

MessPresenter::MessPresenter(MessView& v)
    : view(v)
{

}

void MessPresenter::activate()
{

}

void MessPresenter::deactivate()
{

}

void MessPresenter::setBacklightValuePresenter(int value)
{
    model->setBacklightValue(value);
}


int MessPresenter::readEEPROMvalue()
{
    return model-> ReadValueFromEEPORM();
}
int MessPresenter::readEEPROMCount()
{
    return model-> ReadCountEEPORM();
}
void MessPresenter::sendEEPROMvalue(int value)
{
  model->SendVlaueToEEPROM(value);
}

//void MessPresenter::writeUpol2Image( float value)
//{
//  model->updateUpol(value);
//}


void MessPresenter::setStateSDCS(bool state_SDCS_model)
{
  view.setStateSDCS(state_SDCS_model);
}


void MessPresenter::CANIntReceived(uint32_t ID, const int16_t *data)
{
    view.CANIntReceived(ID, data);
}

void MessPresenter::sendLINControlFrame(uint8_t hv_on, uint8_t regulator, uint8_t current, uint8_t voltage, uint8_t blower)
{
    model->sendLINControlFrame(hv_on, regulator, current, voltage, blower);
}

void MessPresenter::receiveLINStatusFrame()
{
    model->receiveLINStatusFrame();
}

void MessPresenter::onLINStatusReceived(uint8_t *status)
{
    view.updateLINStatus(status);
}

void MessPresenter::setHviValue(uint8_t value)
{

}

