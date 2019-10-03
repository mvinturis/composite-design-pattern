#pragma once

#include "Animation.hpp"

class LoopAnimation : public Animation {
public:
	LoopAnimation(std::shared_ptr<Animation> animation, int loops = 0)
		: Animation("")
		, animation(animation)
		, loops(loops)
	{
	}

	virtual ~LoopAnimation()
	{
	}

	virtual void stop()
	{
		Animation::stop();
		animation->stop();
	}

	virtual void setNotifyValueChangedCallback(NotifyValueChangedCallback notify)
	{
		animation->setNotifyValueChangedCallback(notify);
	}

protected:

	virtual void threadProc()
	{
		int countLoops = loops;
	
		while (!stop_flag && (loops <= 0 || countLoops > 0))
		{
			animation->start();
			animation->waitToFinish();

			if (loops > 0)
				countLoops--;
		}

        if (finished)
            finished();
	}

	std::shared_ptr<Animation> animation;
	int loops;

};
