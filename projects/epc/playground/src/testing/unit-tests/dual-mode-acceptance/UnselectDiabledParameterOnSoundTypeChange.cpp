#include <testing/TestHelper.h>
#include <testing/unit-tests/mock/EditBufferNamedLogicalParts.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include <proxies/hwui/HWUI.h>

TEST_CASE_METHOD(TestHelper::ApplicationFixture,"FROM Layer")
{
  using namespace C15::PID;
  MockPresetStorage presets;

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);
  ebUseCases.load(presets.getLayerPreset());

  {
    auto scope = TestHelper::createTestScope();
    Application::get().getVGManager()->setCurrentVoiceGroup(scope->getTransaction(), VoiceGroup::II, true);
  }

  CHECK(eb->getType() == SoundType::Layer);

  WHEN("FB A/B x selected")
  {
    ebUseCases.selectParameter({ FB_Mix_Osc, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::I });
    }

    THEN("Split Preset Loaded")
    {
      ebUseCases.load(presets.getSplitPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::II });
    }

    THEN("Split converted")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::II });
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::I });
    }
  }

  WHEN("FB A/B x - A-B selected")
  {
    ebUseCases.selectParameter({ FB_Mix_Osc, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::I });
    }

    THEN("Split Preset Loaded")
    {
      ebUseCases.load(presets.getSplitPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::II });
    }

    THEN("Split converted")
    {
      ebUseCases.convertToSplit(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::II });
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_Osc, VoiceGroup::I });
    }
  }

  WHEN("FB: Comb - From x selected")
  {
    ebUseCases.selectParameter({ FB_Mix_Comb_Src, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_Comb, VoiceGroup::I });
    }

    THEN("Split Preset Loaded")
    {
      ebUseCases.load(presets.getSplitPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_Comb, VoiceGroup::II });
    }

    THEN("Split converted")
    {
      ebUseCases.convertToSplit(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_Comb, VoiceGroup::II });
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::I);
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_Comb, VoiceGroup::I });
    }
  }

  WHEN("FB: SV Filter - From x selected")
  {
    ebUseCases.selectParameter({ FB_Mix_SVF_Src, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_SVF, VoiceGroup::I });
    }

    THEN("Split Preset Loaded")
    {
      ebUseCases.load(presets.getSplitPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_SVF, VoiceGroup::II });
    }

    THEN("Split converted")
    {
      ebUseCases.convertToSplit(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_SVF, VoiceGroup::II });
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_SVF, VoiceGroup::I });
    }
  }

  WHEN("FB: Effects - From x selected")
  {
    ebUseCases.selectParameter({ FB_Mix_FX_Src, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_FX, VoiceGroup::I });
    }

    THEN("Split Preset Loaded")
    {
      ebUseCases.load(presets.getSplitPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_FX_Src, VoiceGroup::II });
    }

    THEN("Split converted")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { FB_Mix_FX_Src, VoiceGroup::II });
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::I);
      CHECK(eb->getSelected(VoiceGroup::I)->getID() == ParameterId { FB_Mix_FX, VoiceGroup::I });
    }
  }

  WHEN("Part: Volume selected")
  {
    ebUseCases.selectParameter({ Voice_Grp_Volume, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Volume, VoiceGroup::Global });
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Volume, VoiceGroup::Global });
    }
  }

  WHEN("Part: Tune selected")
  {
    ebUseCases.selectParameter({ Voice_Grp_Tune, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Tune, VoiceGroup::Global });
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Tune, VoiceGroup::Global });
    }
  }

  WHEN("Part: Fade From selected")
  {
    ebUseCases.selectParameter({ Voice_Grp_Fade_From, VoiceGroup::II });

    THEN("Split Preset Loaded")
    {
      ebUseCases.load(presets.getSplitPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId({ Voice_Grp_Volume, VoiceGroup::II }));
    }

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId({ Master_Volume, VoiceGroup::Global }));
    }

    THEN("Split converted")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId({ Voice_Grp_Volume, VoiceGroup::II }));
    }

    THEN("Single converted")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId({ Master_Volume, VoiceGroup::Global }));
    }
  }

  WHEN("Part: Fade Range selected")
  {
    ebUseCases.selectParameter({ Voice_Grp_Fade_Range, VoiceGroup::II });

    THEN("Split Preset Loaded")
    {
      ebUseCases.load(presets.getSplitPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId({ Voice_Grp_Volume, VoiceGroup::II }));
    }

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId({ Master_Volume, VoiceGroup::Global }));
    }

    THEN("Split Convert")
    {
      ebUseCases.convertToSplit(VoiceGroup::I);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId({ Voice_Grp_Volume, VoiceGroup::II }));
    }

    THEN("Single Convert")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId({ Master_Volume, VoiceGroup::Global }));
    }
  }
}

TEST_CASE_METHOD(TestHelper::ApplicationFixture, "Split Loaded")
{
  using namespace C15::PID;
  MockPresetStorage presets;

  auto eb = TestHelper::getEditBuffer();
  EditBufferUseCases ebUseCases(*eb);

  ebUseCases.load(presets.getSplitPreset());

  {
    auto scope = TestHelper::createTestScope();
    Application::get().getVGManager()->setCurrentVoiceGroup(scope->getTransaction(), VoiceGroup::II, true);
  }

  CHECK(eb->getType() == SoundType::Split);

  WHEN("Part: Volume selected")
  {
    ebUseCases.selectParameter({ Voice_Grp_Volume, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Volume, VoiceGroup::Global });
    }

    THEN("Single Convert")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Volume, VoiceGroup::Global });
    }
  }

  WHEN("Part: Tune selected")
  {
    ebUseCases.selectParameter({ Voice_Grp_Tune, VoiceGroup::II });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Tune, VoiceGroup::Global });
    }

    THEN("Single Convert")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Tune, VoiceGroup::Global });
    }
  }

  WHEN("Split Point selected")
  {
    ebUseCases.selectParameter({ Split_Split_Point, VoiceGroup::I });

    THEN("Single Preset Loaded")
    {
      ebUseCases.load(presets.getSinglePreset());
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Volume, VoiceGroup::Global });
    }

    THEN("Layer Preset Loaded")
    {
      ebUseCases.load(presets.getLayerPreset());
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { Voice_Grp_Volume, VoiceGroup::II });
    }

    THEN("Layer Convert")
    {
      ebUseCases.convertToLayer(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::II)->getID() == ParameterId { Voice_Grp_Volume, VoiceGroup::II });
    }

    THEN("Single Convert")
    {
      ebUseCases.convertToSingle(VoiceGroup::II);
      CHECK(eb->getSelected(VoiceGroup::Global)->getID() == ParameterId { Master_Volume, VoiceGroup::Global });
    }
  }
}