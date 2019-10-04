#include "SplitPointParameter.h"

#include <testing/TestDriver.h>
#include <parameters/scale-converters/KeyWithOctaveScaleConverter.h>
#include <testing/TestRootDocument.h>
#include <testing/parameter/TestGroup.h>
#include <testing/parameter/TestParameter.h>
#include "groups/ParameterGroup.h"

SplitPointParameter::SplitPointParameter(ParameterGroup *group, uint16_t id)
    : Parameter(group, id, ScaleConverter::get<KeyWithOctaveScaleConverter>(), 0.5, 60, 60)
{
}

std::string SplitPointParameter::getDisplayValue(VoiceGroup vg) const
{
  auto converter = ScaleConverter::get<KeyWithOctaveScaleConverter>();
  if(vg == VoiceGroup::I)
    return converter->getDimension().stringize(getValue().getRawValue());
  else if(vg == VoiceGroup::II)
    return converter->getDimension().stringize(getNextStepValue(-1, {}));

  return "";
}

void SplitPointParameter::registerTests()
{

  g_test_add_func("/SplitPointParameter/stringize", [] {
    TestRootDocument root;
    TestGroupSet set{ &root };
    TestGroup group(&set, VoiceGroup::Global);
    group.addParameter(new TestParameter<SplitPointParameter>(&group, uint16_t(18700)));

    auto parameter = dynamic_cast<SplitPointParameter *>(group.findParameterByID(18700));
    g_assert(parameter != nullptr);

    auto transScope = UNDO::Scope::startTrashTransaction();
    auto transaction = transScope->getTransaction();

    parameter->stepCPFromHwui(transaction, 1, {});
    g_assert(parameter->getDisplayValue(VoiceGroup::I) == "G3");
    g_assert(parameter->getDisplayValue(VoiceGroup::II) == "F#3");

    parameter->stepCPFromHwui(transaction, 1, {});
    g_assert(parameter->getDisplayValue(VoiceGroup::I) == "G#3");
    g_assert(parameter->getDisplayValue(VoiceGroup::II) == "G3");
  });
}

static TestDriver<SplitPointParameter> driver;