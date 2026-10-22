// Copyright (C) 2026 Intel Corporation
//
// SPDX-License-Identifier: MIT

#include "ortAbiTestFixture.h"

using namespace ovep_tests;

struct ort_abi_provider_options_gdn_hint : public ortAbiSingleDeviceTestFixture {};

TEST_P(ort_abi_provider_options_gdn_hint, accepts_valid_values_for_non_gdn_model) {
  device_wrapper_t device(GetParam());
  for (const auto* value : {"", "true", "false", "TRUE", "FALSE", "1", "0"}) {
    SCOPED_TRACE(value);
    auto network = network_t::create("testdata/mul_1.onnx");
    if (*value != '\0') {
      network.ort_provider_options_["hint_enable_gdn"] = value;
    }
    EXPECT_NO_THROW({ graph_wrapper_t graph(device, network); });
  }
}

TEST_P(ort_abi_provider_options_gdn_hint, rejects_invalid_values) {
  device_wrapper_t device(GetParam());
  for (const auto* value : {"yes", "no", "invalid"}) {
    SCOPED_TRACE(value);
    auto network = network_t::create("testdata/mul_1.onnx");
    network.ort_provider_options_["hint_enable_gdn"] = value;
    try {
      graph_wrapper_t graph(device, network);
      ADD_FAILURE() << "Expected an invalid provider option error";
    } catch (const Ort::Exception& e) {
      EXPECT_THAT(e.what(), testing::HasSubstr("hint_enable_gdn"));
      EXPECT_THAT(e.what(), testing::HasSubstr("Valid values are: 'true', 'false', '1', '0'"));
    }
  }
}

INSTANTIATE_TEST_SUITE_P(ort_abi_provider_options_gdn_hint, ort_abi_provider_options_gdn_hint,
                         ortAbiSingleDeviceTestFixture::all_devices());
