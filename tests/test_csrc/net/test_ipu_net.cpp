// Copyright (c) OpenMMLab. All rights reserved.

// clang-format off
#include "catch.hpp"
// clang-format on

#include "mmdeploy/core/model.h"
#include "mmdeploy/core/net.h"
#include "test_resource.h"

using namespace mmdeploy;

TEST_CASE("test ipu net", "[.ipu_net][resource]") {
  auto& gResource = MMDeployTestResources::Get();
  auto model_list = gResource.LocateModelResources(fs::path{"mmcls"} / "trt");
  REQUIRE(!model_list.empty());

  Model model(model_list.front());
  REQUIRE(model);

  auto backend("ipu");
  auto creator = Registry<Net>::Get().GetCreator(backend);
  REQUIRE(creator);

  Device device{"cpu"};
  auto stream = Stream::GetDefault(device);
  Value net_config{
      {"context", {{"device", device}, {"model", model}, {"stream", stream}}},
      {"name", model.meta().models[0].name},
      {"popef_path",
       "/localdata/cn-customer-engineering/qiangg/cache_poptorch/5299458688024344947.popef"}};
  auto net = creator->Create(net_config);
  REQUIRE(net);
}