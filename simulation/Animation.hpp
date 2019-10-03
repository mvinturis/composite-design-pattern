#pragma once

#include <string>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>


void notifyValueChanged(std::string eventName, std::string newValue);
typedef std::function< decltype(notifyValueChanged) > NotifyValueChangedCallback;

void finished();
typedef std::function< decltype(finished) > FinishedCallback;


class Animation {
public:
	Animation(std::string name)
	: notifyValueChanged(nullptr)
	, finished(nullptr)
	, eventName(name)
	, stop_flag(false)
	, threadStarted(false)
	{
	}

	virtual ~Animation() 
	{
		if (threadStarted)
		{
			t.join();
			threadStarted = false;
		}
	}

	virtual int start()
	{
		threadStarted = true;
		t = std::thread(&Animation::threadProc, this);
        return 0;
	}

	virtual void stop()
	{
		stop_flag = true;
	}

	virtual void waitToFinish()
	{
		if (threadStarted)
		{
			t.join();
			threadStarted = false;
		}
	}

	virtual void setNotifyValueChangedCallback(NotifyValueChangedCallback notify)
	{
		notifyValueChanged = notify;
	}

	virtual void setFinishedCallback(FinishedCallback notify)
	{
		finished = notify;
	}

protected:
	virtual void threadProc() = 0;

	NotifyValueChangedCallback notifyValueChanged;

	FinishedCallback finished;

	std::string eventName;
	bool stop_flag;
	bool threadStarted;
	std::thread t;
};
