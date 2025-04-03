#ifndef MESSPRESENTER_HPP
#define MESSPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MessView;

class MessPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MessPresenter(MessView& v);

    virtual void activate();
    virtual void deactivate();

    void setBacklightValuePresenter(int value);
    void CANIntReceived(uint32_t ID, const int16_t *data) override;

    void sendLINControlFrame(uint8_t hv_on, uint8_t regulator, uint8_t current, uint8_t voltage, uint8_t blower);

    virtual void onLINStatusReceived(uint8_t *status) override;
    //void setLINCurrentValue(uint8_t value);

    void updateLINStatus(uint8_t status);

    void receiveLINStatusFrame();

    //uint8_t getHviValue();
    void setHviValue(uint8_t value);

    virtual int readEEPROMvalue();
    virtual int readEEPROMCount();
    virtual void sendEEPROMvalue(int value);

    void setStateSDCS(bool state_SDCS_model);


    virtual ~MessPresenter() {}

private:
    MessPresenter();

    MessView& view;
};

#endif // MESSPRESENTER_HPP
