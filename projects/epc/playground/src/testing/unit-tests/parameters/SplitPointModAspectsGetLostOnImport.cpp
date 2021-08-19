#include "testing/TestHelper.h"
#include "presets/Preset.h"
#include "presets/PresetManager.h"
#include <serialization/PresetManagerSerializer.h>
#include <testing/unit-tests/mock/MockPresetStorage.h>
#include "CompileTimeOptions.h"
#include <xml/ZippedMemoryOutStream.h>
#include <parameter_declarations.h>
#include <presets/PresetParameter.h>

TEST_CASE("Import Preset with ModAspects on Split")
{
  auto pm = TestHelper::getPresetManager();

  {
    auto trash = UNDO::Scope::startTrashTransaction();
    pm->clear(trash->getTransaction());
    CHECK(pm->getSelectedBank() == nullptr);
  }

  PresetManagerUseCases uc(pm);
  FileInStream stream(getSourceDir() + "/projects/epc/playground/test-resources/Fuxi_Split.xml", false);
  uc.importBankFromStream(stream, 0, 0, "Fuxi-Swarms");

  auto fuxiesSplit = pm->getSelectedBank();
  CHECK(fuxiesSplit != nullptr);

  auto firstPreset = fuxiesSplit->getPresetAt(0);
  CHECK(firstPreset != nullptr);

  auto oldParam = firstPreset->findParameterByID({C15::PID::Split_Split_Point, VoiceGroup::Global}, false);
  CHECK(oldParam == nullptr);

  auto splitPntI = firstPreset->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I }, false);
  auto splitPntII = firstPreset->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II }, false);
  CHECK(splitPntI != nullptr);
  CHECK(splitPntII != nullptr);

  THEN("Modulation Aspects are Present")
  {
    CHECK(splitPntI->getModulationAmount() != 0);
    CHECK(splitPntI->getModulationSource() != MacroControls::NONE);
  }
}
