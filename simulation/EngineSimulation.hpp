#pragma once

#include <algorithm>
#include <functional>
#include <mutex>
#include <condition_variable>
#include "NumberAnimation.hpp"
#include "ParallelAnimation.hpp"
#include "Messages.hpp"

#define GEAR_SHIFT_INTERVAL		100
#define ACCELERATION_INTERVAL	2000

class EngineSimulation : public Animation {
public:

	enum SimulationStep {
		NONE = 0,
		START,
		ACCELERATE_FIRST_GEAR,
		ACCELERATE_SECOND_GEAR,
		ACCELERATE_THIRD_GEAR,
		ACCELERATE_FOURTH_GEAR,
		ACCELERATE_FIFTH_GEAR,
		ACCELERATE_SIXTH_GEAR,
		BREAK_SIXTH_GEAR,
		BREAK_FIFTH_GEAR,
		BREAK_FOURTH_GEAR,
		BREAK_THIRD_GEAR,
		BREAK_SECOND_GEAR,
		BREAK_FIRST_GEAR,
		FINISH
	};

	EngineSimulation()
		: Animation("EngineSimulation")
		, parallelAnimation(nullptr)
		, gear(0)
		, vehicleSpeed(0)
		, engineSpeed(0)
		, step(START)
	{
	}

	virtual ~EngineSimulation() 
	{
	}

protected:

	virtual void threadProc()
	{
		step = ACCELERATE_FIRST_GEAR;
		startNextSimulation();

		std::unique_lock<std::mutex> lock(animationFinishedMutex);

		while (!stop_flag)
		{
			if (signalAnimationFinished.wait_for(lock, std::chrono::seconds(10)) == std::cv_status::no_timeout)
				onAnimationFinished();
		}

		if (parallelAnimation.get()) 
		{
			parallelAnimation->stop();
			parallelAnimation->waitToFinish();
		}

		if (finished)
			finished();
	}

	void startNextSimulation()
	{
		switch (step)
		{
		case ACCELERATE_FIRST_GEAR:
			restartParallelAnimation(0, 30, 1, 6.1, ACCELERATION_INTERVAL);
			break;
		case ACCELERATE_SECOND_GEAR:
			// on shifting gears, we loose a bit of speed and the engine rotation changes
			restartParallelAnimation(30, 26, 6.1, 2.4, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "2");
			restartParallelAnimation(26, 60, 2.4, 5.6, ACCELERATION_INTERVAL);
			break;
		case ACCELERATE_THIRD_GEAR:
			// on shifting gears, we loose a bit of speed and the engine rotation changes
			restartParallelAnimation(60, 56, 5.6, 2.3, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "3");
			restartParallelAnimation(56, 100, 2.3, 5.1, ACCELERATION_INTERVAL);
			break;
		case ACCELERATE_FOURTH_GEAR:
			// on shifting gears, we loose a bit of speed and the engine rotation changes
			restartParallelAnimation(100, 96, 5.1, 2.2, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "4");
			restartParallelAnimation(96, 140, 2.2, 6.2, ACCELERATION_INTERVAL);
			break;
		case ACCELERATE_FIFTH_GEAR:
			// on shifting gears, we loose a bit of speed and the engine rotation changes
			restartParallelAnimation(140, 136, 6.2, 2.3, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "5");
			restartParallelAnimation(136, 170, 2.3, 4.8, ACCELERATION_INTERVAL);
			break;
		case ACCELERATE_SIXTH_GEAR:
			// on shifting gears, we loose a bit of speed and the engine rotation changes
			restartParallelAnimation(170, 169, 4.8, 2.7, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "6");
			restartParallelAnimation(169, 210, 2.7, 5.2, ACCELERATION_INTERVAL);
			break;
		case BREAK_SIXTH_GEAR:
			// engine break, no gear shifting
			restartParallelAnimation(210, 205, 5.2, 4.7, GEAR_SHIFT_INTERVAL, true);
			restartParallelAnimation(205, 170, 4.7, 2.5, ACCELERATION_INTERVAL);
			break;
		case BREAK_FIFTH_GEAR:
			restartParallelAnimation(170, 169, 2.5, 4.9, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "5");
			restartParallelAnimation(169, 140, 4.9, 2.3, ACCELERATION_INTERVAL);
			break;
		case BREAK_FOURTH_GEAR:
			restartParallelAnimation(140, 139, 2.3, 4.2, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "4");
			restartParallelAnimation(139, 100, 4.2, 2.3, ACCELERATION_INTERVAL);
			break;
		case BREAK_THIRD_GEAR:
			restartParallelAnimation(100, 99, 2.3, 4.1, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "3");
			restartParallelAnimation(99, 60, 4.1, 2.2, ACCELERATION_INTERVAL);
			break;
		case BREAK_SECOND_GEAR:
			restartParallelAnimation(60, 59, 2.2, 3.2, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "2");
			restartParallelAnimation(59, 30, 3.2, 1.9, ACCELERATION_INTERVAL);
			break;
		case BREAK_FIRST_GEAR:
			restartParallelAnimation(30, 29, 1.9, 2.8, GEAR_SHIFT_INTERVAL, true);
			notifyValueChanged(GEAR_SHIFT, "1");
			restartParallelAnimation(29, 0, 2.8, 1, ACCELERATION_INTERVAL);
			break;
		default:
			break;
		}
	}

	void restartParallelAnimation(double vehicleSpeedFrom, double vehicleSpeedTo, double engineSpeedFrom, double engineSpeedTo, int duration, bool synchron = false) 
	{
		parallelAnimation = std::make_shared<ParallelAnimation>(
			std::make_shared<NumberAnimation>(VEHICLE_SPEED, vehicleSpeedFrom, vehicleSpeedTo, duration), 
			std::make_shared<NumberAnimation>(ENGINE_SPEED, engineSpeedFrom, engineSpeedTo, duration, 1)); 

		parallelAnimation->setNotifyValueChangedCallback(notifyValueChanged);
		if (!synchron)
		{
			parallelAnimation->setFinishedCallback(std::bind(&EngineSimulation::notifyAnimationFinished, this));
		}

		parallelAnimation->start();
		if (synchron)
		{
			parallelAnimation->waitToFinish();
		}
	}

	void notifyAnimationFinished()
	{
		signalAnimationFinished.notify_one();
	}

	void onAnimationFinished()
	{
		((int&)step)++;
		if (step >= FINISH)
		{
			notifyValueChanged(GEAR_SHIFT, "0");
			stop_flag = true;
			return;
		}

		startNextSimulation();
	}

	std::shared_ptr<ParallelAnimation> parallelAnimation;

	int gear;
	double vehicleSpeed;
	double engineSpeed;

	SimulationStep step;

	std::mutex animationFinishedMutex;
	std::condition_variable signalAnimationFinished;
};
