#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../simulation/ParallelAnimation.hpp"
#include "mock/MockAnimation.hpp"
#include "mock/MockAnimationNotifier.hpp"

using namespace testing;

class ParallelAnimationUnitTests : public ::testing::Test 
{
protected:
    void SetUp() override {
        mockAnimation1 = std::make_shared<MockAnimation>();
        mockAnimation2 = std::make_shared<MockAnimation>();
        mockAnimation3 = std::make_shared<MockAnimation>();
        mockAnimation4 = std::make_shared<MockAnimation>();
        mockAnimationNotifier = new MockAnimationNotifier();
    }

    void TearDown() override {
        if (mockAnimationNotifier)
        {
            delete mockAnimationNotifier;
            mockAnimationNotifier = nullptr;
        }
        mockAnimation4.reset();
        mockAnimation3.reset();
        mockAnimation2.reset();
        mockAnimation1.reset();
    }

    void testWaitForFinish(std::shared_ptr<MockAnimation> animation1,
        std::shared_ptr<MockAnimation> animation2,
        std::shared_ptr<MockAnimation> animation3,
        std::shared_ptr<MockAnimation> animation4);

    MockAnimationNotifier *mockAnimationNotifier;
    std::shared_ptr<MockAnimation> mockAnimation1;
    std::shared_ptr<MockAnimation> mockAnimation2;
    std::shared_ptr<MockAnimation> mockAnimation3;
    std::shared_ptr<MockAnimation> mockAnimation4;
};

void ParallelAnimationUnitTests::testWaitForFinish(
    std::shared_ptr<MockAnimation> animation1,
    std::shared_ptr<MockAnimation> animation2,
    std::shared_ptr<MockAnimation> animation3,
    std::shared_ptr<MockAnimation> animation4)
{
    if (animation1)    
    {
        EXPECT_CALL(*(animation1.get()), start()).Times(1);
        EXPECT_CALL(*(animation1.get()), waitToFinish()).Times(1);
        EXPECT_CALL(*(animation1.get()), setNotifyValueChangedCallback(_)).Times(1);
        EXPECT_CALL(*(animation1.get()), setFinishedCallback(_)).Times(1);
        EXPECT_CALL(*(animation1.get()), threadProc()).Times(1);
        ON_CALL(*(animation1.get()), threadProc()).WillByDefault(Invoke([](){
            std::cout << "animation1 ON_CALL threadProc, sleep 1 second" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "animation1 threadProc done" << std::endl;
        }));
    }
    if (animation2)    
    {
        EXPECT_CALL(*(animation2.get()), start()).Times(1);
        EXPECT_CALL(*(animation2.get()), waitToFinish()).Times(1);
        EXPECT_CALL(*(animation2.get()), setNotifyValueChangedCallback(_)).Times(1);
        EXPECT_CALL(*(animation2.get()), setFinishedCallback(_)).Times(1);
        EXPECT_CALL(*(animation2.get()), threadProc()).Times(1);
        ON_CALL(*(animation2.get()), threadProc()).WillByDefault(Invoke([](){
            std::cout << "animation2 ON_CALL threadProc, sleep 2 seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "animation2 threadProc done" << std::endl;
        }));
    }
    if (animation3)    
    {
        EXPECT_CALL(*(animation3.get()), start()).Times(1);
        EXPECT_CALL(*(animation3.get()), waitToFinish()).Times(1);
        EXPECT_CALL(*(animation3.get()), setNotifyValueChangedCallback(_)).Times(1);
        EXPECT_CALL(*(animation3.get()), setFinishedCallback(_)).Times(1);
        EXPECT_CALL(*(animation3.get()), threadProc()).Times(1);
        ON_CALL(*(animation3.get()), threadProc()).WillByDefault(Invoke([](){
            std::cout << "animation3 ON_CALL threadProc, sleep 3 seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            std::cout << "animation3 threadProc done" << std::endl;
        }));
    }
    if (animation4)    
    {
        EXPECT_CALL(*(animation4.get()), start()).Times(1);
        EXPECT_CALL(*(animation4.get()), waitToFinish()).Times(1);
        EXPECT_CALL(*(animation4.get()), setNotifyValueChangedCallback(_)).Times(1);
        EXPECT_CALL(*(animation4.get()), setFinishedCallback(_)).Times(1);
        EXPECT_CALL(*(animation4.get()), threadProc()).Times(1);
        ON_CALL(*(animation4.get()), threadProc()).WillByDefault(Invoke([](){
            std::cout << "animation4 ON_CALL threadProc, sleep 4 seconds" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(4));
            std::cout << "animation4 threadProc done" << std::endl;
        }));
    }

    ParallelAnimation parallelAnimation(animation1, animation2, animation3, animation4);

    parallelAnimation.setNotifyValueChangedCallback(
        std::bind(&MockAnimationNotifier::notifyValueChanged, mockAnimationNotifier, std::placeholders::_1, std::placeholders::_2));

    parallelAnimation.setFinishedCallback(
        std::bind(&MockAnimationNotifier::finished, mockAnimationNotifier));

    parallelAnimation.start();
    parallelAnimation.waitToFinish();
}

TEST_F(ParallelAnimationUnitTests, TestOneAnimation) {
    testWaitForFinish(mockAnimation1, nullptr, nullptr, nullptr);
}

TEST_F(ParallelAnimationUnitTests, TestTwoAnimations) {
    testWaitForFinish(mockAnimation1, mockAnimation2, nullptr, nullptr);
}

TEST_F(ParallelAnimationUnitTests, TestThreeAnimations) {
    testWaitForFinish(mockAnimation1, mockAnimation2, mockAnimation3, nullptr);
}

TEST_F(ParallelAnimationUnitTests, TestFourAnimations) {
    testWaitForFinish(mockAnimation1, mockAnimation2, mockAnimation3, mockAnimation4);
}
