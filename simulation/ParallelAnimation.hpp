#pragma once

#include <list>
#include <atomic>
#include "Animation.hpp"

class ParallelAnimation : public Animation {
public:
	ParallelAnimation(std::shared_ptr<Animation> animation1, 
		std::shared_ptr<Animation> animation2 = nullptr, 
		std::shared_ptr<Animation> animation3 = nullptr, 
		std::shared_ptr<Animation> animation4 = nullptr)
        : Animation("ParallelAnimation")
	{
		if (animation1 != nullptr)
			animationList.push_back(animation1);
		if (animation2 != nullptr)
			animationList.push_back(animation2);
		if (animation3 != nullptr)
			animationList.push_back(animation3);
		if (animation4 != nullptr)
			animationList.push_back(animation4);
	}
	virtual ~ParallelAnimation()
	{
	}

	virtual int start()
	{
		waitToFinishCount = (int)animationList.size();

		std::for_each(animationList.begin(), animationList.end(),
			[this](std::shared_ptr<Animation> animation)
			{
				if (this->finished)
				{
					animation->setFinishedCallback(std::bind(&ParallelAnimation::onAnimationFinished, this));
				}
				animation->start();
			});

		return 0;
	}

	virtual void waitToFinish()
	{
		std::for_each(animationList.begin(), animationList.end(),
			[](std::shared_ptr<Animation> animation)
			{
				animation->waitToFinish();
			});
	}

	virtual void stop()
	{
		std::for_each(animationList.begin(), animationList.end(),
			[](std::shared_ptr<Animation> animation)
			{
				animation->stop();
			});
	}

	virtual void setNotifyValueChangedCallback(NotifyValueChangedCallback notify)
	{
		std::for_each(animationList.begin(), animationList.end(),
			[notify](std::shared_ptr<Animation> animation)
			{
				animation->setNotifyValueChangedCallback(notify);
			});
	}

protected:

	virtual void threadProc()
	{
	}

	void onAnimationFinished()
	{
		int count = waitToFinishCount.fetch_sub(1) - 1;
		if (count <= 0)
		{
			if (finished)
				finished();
		}
	}

	std::atomic<int> waitToFinishCount;
	std::list< std::shared_ptr<Animation> > animationList;

};
