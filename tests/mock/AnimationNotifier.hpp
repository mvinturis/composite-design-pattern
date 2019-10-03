#pragma once

class AnimationNotifier 
{
public:
    virtual ~AnimationNotifier() {
    }

    virtual void notifyValueChanged(std::string eventName, std::string newValue) = 0;

    virtual void finished() = 0;
};
