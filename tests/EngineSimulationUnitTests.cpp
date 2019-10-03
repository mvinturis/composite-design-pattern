#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../simulation/EngineSimulation.hpp"
#include "mock/MockAnimationNotifier.hpp"

using namespace testing;


class EngineSimulationUnitTests : public ::testing::Test 
{
protected:
    void SetUp() override {
        engineSimulation = new EngineSimulation();
        mockAnimationNotifier = new MockAnimationNotifier();

        engineSimulation->setNotifyValueChangedCallback(
            std::bind(&MockAnimationNotifier::notifyValueChanged, mockAnimationNotifier, std::placeholders::_1, std::placeholders::_2));

        engineSimulation->setFinishedCallback(
            std::bind(&MockAnimationNotifier::finished, mockAnimationNotifier));
    }

    void TearDown() override {
        if (mockAnimationNotifier)
        {
            delete mockAnimationNotifier;
            mockAnimationNotifier = nullptr;
        }
        if (engineSimulation)
        {
            delete engineSimulation;
            engineSimulation = nullptr;
        }
    }

    EngineSimulation *engineSimulation;
    MockAnimationNotifier *mockAnimationNotifier;
};

TEST_F(EngineSimulationUnitTests, TestWaitToFinish) {
    EXPECT_CALL(*mockAnimationNotifier, notifyValueChanged(Truly([](std::string eventName) -> bool {
        std::cout << eventName;
        return true;
    }), Truly([](std::string newValue) -> bool {
        std::cout << " " << newValue << std::endl;
        return true;
    }))).Times(AtLeast(1));

    EXPECT_CALL(*mockAnimationNotifier, finished()).Times(1);

    engineSimulation->start();
    engineSimulation->waitToFinish();
}
