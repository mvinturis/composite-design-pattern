#pragma once

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "CarSimulatorNotifier.hpp"

class MockCarSimulatorNotifier : public CarSimulatorNotifier {
 public:
  MOCK_METHOD1(carSimulatorValueChanged,
      int(struct prop_info_t *property_info));
};
