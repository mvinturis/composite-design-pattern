#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../simulation/LoopAnimation.hpp"
#include "mock/MockAnimation.hpp"
#include "mock/MockAnimationNotifier.hpp"

using namespace testing;

class LoopAnimationUnitTests : public ::testing::Test 
{
protected:
    void SetUp() override {
        mockAnimation = std::make_shared<MockAnimation>();
        mockAnimationNotifier = new MockAnimationNotifier();

        loopAnimation = new LoopAnimation(mockAnimation);

        EXPECT_CALL(*(mockAnimation.get()), setNotifyValueChangedCallback(_)).Times(1);

        loopAnimation->setNotifyValueChangedCallback(
            std::bind(&MockAnimationNotifier::notifyValueChanged, mockAnimationNotifier, std::placeholders::_1, std::placeholders::_2));

        loopAnimation->setFinishedCallback(
            std::bind(&MockAnimationNotifier::finished, mockAnimationNotifier));
    }

    void TearDown() override {
        if (loopAnimation)
        {
            delete loopAnimation;
            loopAnimation = nullptr;
        }
        if (mockAnimationNotifier)
        {
            delete mockAnimationNotifier;
            mockAnimationNotifier = nullptr;
        }
        mockAnimation.reset();
    }

    LoopAnimation *loopAnimation;
    MockAnimationNotifier *mockAnimationNotifier;
    std::shared_ptr<MockAnimation> mockAnimation;
};


TEST_F(LoopAnimationUnitTests, TestLoops) {
    int loops = 0;
    for (loops = 1; loops < 10; loops++)
    {
        //initialization
        LoopAnimation *loopAnimation1 = new LoopAnimation(mockAnimation, loops);

        EXPECT_CALL(*(mockAnimation.get()), start()).Times(loops);
        EXPECT_CALL(*(mockAnimation.get()), waitToFinish()).Times(loops);
        EXPECT_CALL(*(mockAnimation.get()), setNotifyValueChangedCallback(_)).Times(1);
        EXPECT_CALL(*(mockAnimation.get()), threadProc()).Times(loops);
        EXPECT_CALL(*mockAnimationNotifier, finished()).Times(1);

        loopAnimation1->setNotifyValueChangedCallback(
            std::bind(&MockAnimationNotifier::notifyValueChanged, mockAnimationNotifier, std::placeholders::_1, std::placeholders::_2));

        loopAnimation1->setFinishedCallback(
            std::bind(&MockAnimationNotifier::finished, mockAnimationNotifier));

        loopAnimation1->start();
        loopAnimation1->waitToFinish();

        // validate expectations
        Mock::VerifyAndClearExpectations(mockAnimation.get());
        Mock::VerifyAndClearExpectations(mockAnimationNotifier);

        // cleanup
        if (loopAnimation1)
        {
            delete loopAnimation1;
            loopAnimation1 = nullptr;
        }
    }
}

TEST_F(LoopAnimationUnitTests, TestInfiniteLoops) 
{
    int loops_until_stop = 12;
    EXPECT_CALL(*(mockAnimation.get()), start()).Times(loops_until_stop);
    EXPECT_CALL(*(mockAnimation.get()), stop()).Times(1);
    EXPECT_CALL(*(mockAnimation.get()), waitToFinish()).Times(loops_until_stop);
    EXPECT_CALL(*(mockAnimation.get()), threadProc()).Times(loops_until_stop);
    ON_CALL(*(mockAnimation.get()), threadProc()).WillByDefault(Invoke([](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "ON_CALL threadProc, sleep 1 second" << std::endl;
    }));
    EXPECT_CALL(*mockAnimationNotifier, finished()).Times(1);

    loopAnimation->start();
    std::this_thread::sleep_for(std::chrono::seconds(loops_until_stop));
    loopAnimation->stop();
    loopAnimation->waitToFinish();
}
