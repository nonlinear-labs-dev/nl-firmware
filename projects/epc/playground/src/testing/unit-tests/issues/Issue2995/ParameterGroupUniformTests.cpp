#include <testing/TestHelper.h>
#include <presets/EditBuffer.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "uniform paramter types for each group")
{
  for(auto vg : { VoiceGroup::I, VoiceGroup::II, VoiceGroup::Global })
  {
    for(auto &g : app->getPresetManager()->getEditBuffer()->getParameterGroups(vg))
    {
      bool hasAny = false;
      bool isPoly = false;
      bool isMono = false;
      bool isGlobal = false;

      for(const auto &p : g->getParameters())
      {
        auto foundType = p->getType();
        auto isPolyParam = foundType == C15::Descriptors::ParameterType::Polyphonic_Unmodulateable
            || foundType == C15::Descriptors::ParameterType::Polyphonic_Modulateable;
        auto isMonoParam = foundType == C15::Descriptors::ParameterType::Monophonic_Unmodulateable
            || foundType == C15::Descriptors::ParameterType::Monophonic_Modulateable;
        auto isGlobalParam = foundType == C15::Descriptors::ParameterType::Global_Modulateable
            || foundType == C15::Descriptors::ParameterType::Global_Unmodulateable;

        if(!hasAny)
        {
          hasAny = true;
          isPoly = isPolyParam;
          isMono = isMonoParam;
          isGlobal = isGlobalParam;
        }
        else
        {
          nltools_detailedAssertAlways(isPoly == isPolyParam, "poly not consistent");
          nltools_detailedAssertAlways(isMono == isMonoParam, "mono not consistent");
          nltools_detailedAssertAlways(isGlobal == isGlobalParam, "global not consistent");
        }
      }
    }
  }
}