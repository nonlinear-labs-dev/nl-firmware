#include <catch.hpp>
#include "synth/c15-audio-engine/parameter_handle.h"
#include "nltools/logging/Log.h"

TEST_CASE("MC_Assignment")
{
  Engine::Handle::ParameterHandle handle {};
  auto &assignment = handle.m_global.m_parameters.m_assignment;

  WHEN("One Param Mapped")
  {
    const auto paramId = (uint32_t) C15::Parameters::Global_Modulateables::Master_Pan;
    const auto mcId = (uint32_t) C15::Parameters::Macro_Controls::MC_A;

    assignment.reassign(paramId, mcId);

    int numModulated = 0;

    for(assignment.first(mcId); assignment.running(); assignment.next())
      numModulated++;

    const auto firstPass = numModulated;

    REQUIRE(numModulated == 1);

    numModulated = 0;

    for(assignment.first(mcId); assignment.running(); assignment.next())
      numModulated++;

    REQUIRE(numModulated == firstPass);
  }

  WHEN("Two Params Mapped")
  {
    const auto paramId1 = (uint32_t) C15::Parameters::Global_Modulateables::Master_Pan;
    const auto paramId2 = (uint32_t) C15::Parameters::Global_Modulateables::Master_Volume;
    const auto mcId = (uint32_t) C15::Parameters::Macro_Controls::MC_A;

    assignment.reassign(paramId1, mcId);
    assignment.reassign(paramId2, mcId);

    int numModulated = 0;

    for(assignment.first(mcId); assignment.running(); assignment.next())
      numModulated++;

    const auto firstPass = numModulated;

    REQUIRE(numModulated == 2);

    numModulated = 0;

    for(assignment.first(mcId); assignment.running(); assignment.next())
      numModulated++;

    REQUIRE(numModulated == firstPass);

    WHEN("Breaking out of Iteration")
    {
      numModulated = 0;

      for(assignment.first(mcId); assignment.running(); assignment.next())
      {
        numModulated++;
        break;
      }

      CHECK(numModulated == 1);

      numModulated = 0;

      for(assignment.first(mcId); assignment.running(); assignment.next())
        numModulated++;

      CHECK(numModulated == 2);
    }
  }
}
