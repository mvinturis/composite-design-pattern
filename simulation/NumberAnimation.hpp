#pragma once

#include <cmath>
#include "Animation.hpp"

class NumberAnimation : public Animation {
public:
	NumberAnimation(std::string eventName, double from, double to, int duration, int precision = 0) // duration in milliseconds 
		: Animation(eventName)
		, from(from)
		, to(to)
		, notifyValue(0)
		, duration(duration)
		, precision(precision)
	{
	}

	virtual ~NumberAnimation()
	{
	}

	void init(double from, double to, int duration, int precision = 0) 
	{
		this->from = from;
		this->to = to;
		this->duration = duration;
		this->precision = precision;
		this->notifyValue = 0;
	}

protected:

	virtual void threadProc()
	{
		int interval = getSleepInterval();
        	double increment = getStepIncrement();

		notifyValue = from;

		if (notifyValueChanged)
			notifyValueChanged(eventName, std::to_string(notifyValue));
		notifyValue += increment;

		while (!stop_flag && ((from < to && notifyValue <= to) || (from > to && notifyValue => to)))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));

			if (notifyValueChanged)
				notifyValueChanged(eventName, std::to_string(notifyValue));
			notifyValue += increment;
		}

		if (finished)
			finished();
	}

	int getSleepInterval()
	{
		double precisionFactor = std::pow(10, precision);

		if (from == to)
			return 0;

		if (from < to)
			return int((double)duration / ((to - from) * precisionFactor));

		return  int((double)duration / ((from - to) * precisionFactor));
	}

	double getStepIncrement()
	{
		double precisionFactor = std::pow(10, precision);

		if (from <= to)
	    		return 1 / precisionFactor;
		return -1 / precisionFactor;
	}

	double from;
	double to;
	double notifyValue;
	int duration;  // duration in milliseconds 
	int precision;

};
