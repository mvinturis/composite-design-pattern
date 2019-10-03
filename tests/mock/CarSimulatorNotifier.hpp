#pragma once

class CarSimulatorNotifier 
{
public:
    virtual ~CarSimulatorNotifier() {
    }

    virtual int carSimulatorValueChanged(struct prop_info_t *property_info) = 0;
};
