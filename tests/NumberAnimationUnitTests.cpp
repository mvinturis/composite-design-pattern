#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../simulation/NumberAnimation.hpp"
#include "mock/MockAnimationNotifier.hpp"

using namespace testing;

class NumberAnimationUnitTests : public ::testing::Test 
{
protected:
    void SetUp() override {
        mockAnimationNotifier = new MockAnimationNotifier();
    }

    void TearDown() override {
        if (mockAnimationNotifier)
        {
            delete mockAnimationNotifier;
            mockAnimationNotifier = nullptr;
        }
    }

    void testWaitForFinish(int from, int to, int duration, int precision = 0);

    MockAnimationNotifier *mockAnimationNotifier;
};

void NumberAnimationUnitTests::testWaitForFinish(int from, int to, int duration, int precision /*= 0*/)
{
    int currentValue = from;
    std::string eventName = "value changed";

    EXPECT_CALL(*mockAnimationNotifier, notifyValueChanged(eventName.c_str(), 
        Truly([&from, &to, &currentValue, &eventName](std::string value) -> bool {
            int newValue = std::atoi(value.c_str());
            bool result = (from <= to && newValue <= to && newValue >= currentValue) 
                            || (from > to && newValue >= to && newValue <= currentValue);
            currentValue = newValue;
            std::cout << eventName << " " << newValue << std::endl;
            return result;
        }))).Times(AtLeast(1));

    EXPECT_CALL(*mockAnimationNotifier, finished()).Times(1);

    NumberAnimation numberAnimation(eventName, from, to, duration, precision);

    numberAnimation.setNotifyValueChangedCallback(
        std::bind(&MockAnimationNotifier::notifyValueChanged, mockAnimationNotifier, std::placeholders::_1, std::placeholders::_2));

    numberAnimation.setFinishedCallback(
        std::bind(&MockAnimationNotifier::finished, mockAnimationNotifier));

    numberAnimation.start();
    numberAnimation.waitToFinish();
}

TEST_F(NumberAnimationUnitTests, TestAscendingIntervals) {
    testWaitForFinish(3, 3, 1000);
    testWaitForFinish(0, 30, 1000);
    testWaitForFinish(5, 10, 1000, 1);
}

TEST_F(NumberAnimationUnitTests, TestDescendingIntervals) {
    testWaitForFinish(30, 0, 1000);
    testWaitForFinish(10, 5, 1000, 1);
}
