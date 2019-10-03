#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../simulation/Animation.hpp"

class MockAnimation : public Animation {
 public:
  MockAnimation() 
    : Animation("mock") 
  {
    ON_CALL(*this, start).WillByDefault([this]() -> int {
        return Animation::start();
    });
    ON_CALL(*this, stop).WillByDefault([this]() {
        Animation::stop();
    });
    ON_CALL(*this, waitToFinish).WillByDefault([this]() {
        Animation::waitToFinish();
    });
    ON_CALL(*this, setNotifyValueChangedCallback).WillByDefault([this](NotifyValueChangedCallback notify) {
        Animation::setNotifyValueChangedCallback(notify);
    });
    ON_CALL(*this, setFinishedCallback).WillByDefault([this](FinishedCallback notify) {
        Animation::setFinishedCallback(notify);
    });
  }
  MOCK_METHOD0(start,
      int());
  MOCK_METHOD0(stop,
      void());
  MOCK_METHOD0(waitToFinish,
      void());
  MOCK_METHOD1(setNotifyValueChangedCallback,
      void(NotifyValueChangedCallback notify));
  MOCK_METHOD1(setFinishedCallback,
      void(FinishedCallback notify));
  MOCK_METHOD0(threadProc,
      void());
};
