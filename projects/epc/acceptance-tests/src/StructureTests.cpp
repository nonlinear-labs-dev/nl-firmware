#include <catch.hpp>
#include <synth/c15-audio-engine/mappable_list.h>
#include "synth/c15-audio-engine/parameter_storage.h"
#include "synth/c15-audio-engine/parameter_handle.h"
#include "nltools/logging/Log.h"

TEST_CASE("MC_Assignment")
{
  using Global_Storage = Engine::Storage::Global_Parameter_Storage<
      C15::Parameters::Hardware_Sources, C15::Parameters::Hardware_Amounts, C15::Parameters::Macro_Controls,
      C15::Parameters::Global_Modulateables, C15::Parameters::Global_Unmodulateables>;

  Engine::Param_Handle m_params;

  WHEN("One Param Mapped")
  {
    auto param = C15::Parameters::Global_Modulateables::Master_Pan;
    auto mc = C15::Parameters::Macro_Controls::MC_A;
    m_params.m_global.m_assignment.reassign(param, mc);

    int numModulated = 0;
    for(auto modParam = m_params.globalChainFirst(1); modParam; modParam = m_params.globalChainNext())
    {
      numModulated++;
    }

    auto firstPass = numModulated;
    REQUIRE(numModulated == 1);

    numModulated = 0;
    for(auto modParam = m_params.globalChainFirst(1); modParam; modParam = m_params.globalChainNext())
    {
      numModulated++;
    }
    REQUIRE(numModulated == firstPass);
  }

  WHEN("Two Params Mapped")
  {
    auto param1 = C15::Parameters::Global_Modulateables::Master_Pan;
    auto param2 = C15::Parameters::Global_Modulateables::Master_Volume;
    auto mc = C15::Parameters::Macro_Controls::MC_A;
    m_params.m_global.m_assignment.reassign(param1, mc);
    m_params.m_global.m_assignment.reassign(param2, mc);

    int numModulated = 0;
    for(auto modParam = m_params.globalChainFirst(1); modParam; modParam = m_params.globalChainNext())
    {
      numModulated++;
    }

    auto firstPass = numModulated;
    REQUIRE(numModulated == 2);

    numModulated = 0;
    for(auto modParam = m_params.globalChainFirst(1); modParam; modParam = m_params.globalChainNext())
    {
      numModulated++;
    }

    REQUIRE(numModulated == firstPass);

    WHEN("Iterating is breaked")
    {
      numModulated = 0;
      for(auto modParam = m_params.globalChainFirst(1); modParam; modParam = m_params.globalChainNext())
      {
        numModulated++;
        break;
      }

      CHECK(numModulated == 1);

      numModulated = 0;
      for(auto modParam = m_params.globalChainFirst(1); modParam; modParam = m_params.globalChainNext())
      {
        numModulated++;
      }
      CHECK(numModulated == 2);
    }
  }
}