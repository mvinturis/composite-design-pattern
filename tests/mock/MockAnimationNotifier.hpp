#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "AnimationNotifier.hpp"

class MockAnimationNotifier : public AnimationNotifier {
 public:
  MOCK_METHOD2(notifyValueChanged,
      void(std::string eventName, std::string newValue));
  MOCK_METHOD0(finished,
      void());
};
