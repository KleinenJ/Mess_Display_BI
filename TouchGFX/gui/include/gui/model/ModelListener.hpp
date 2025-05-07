#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <stdint.h>

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual void CANIntReceived(uint32_t ID, const int16_t *data) {}
    virtual void CANFloatReceived(uint32_t ID, const float *data) {}
    virtual void onLINStatusReceived(uint8_t* status) {}
    virtual ~ModelListener() {}
    //virtual void getHviValue(uint32_t value) {}
    //virtual void setHviValue(uint8_t value) {}
    virtual void setStateSDCS(bool state_SDCS_model) {}

    void bind(Model* m)
    {
        model = m;
    }
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
