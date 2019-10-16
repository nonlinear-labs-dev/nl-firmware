#include "SplitPointParameter.h"

#include <testing/TestDriver.h>
#include <parameters/scale-converters/KeyWithOctaveScaleConverter.h>
#include <testing/TestRootDocument.h>
#include <testing/parameter/TestGroup.h>
#include <testing/parameter/TestParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/DualSpecialParameterScreen.h>
#include "groups/ParameterGroup.h"

SplitPointParameter::SplitPointParameter(ParameterGroup *group, uint16_t id)
    : Parameter(group, id, ScaleConverter::get<KeyWithOctaveScaleConverter>(), 0.5, 60, 60)
{
}

ustring SplitPointParameter::getGroupAndParameterName() const
{
  return "Split Point";
}

DFBLayout *SplitPointParameter::createLayout(FocusAndMode focusAndMode) const
{
  return new DualSpecialParameterScreen();
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

ustring SplitPointParameter::getDisplayString() const
{
  auto currentVG = Application::get().getVoiceGroupSelectionHardwareUI()->getEditBufferSelection();
  if(currentVG == VoiceGroup::I)
    return getDisplayValue(VoiceGroup::I);
  else
    return getDisplayValue(VoiceGroup::II);
}

void SplitPointParameter::registerTests()
{

  g_test_add_func("/SplitPointParameter/stringize", [] {
    TestRootDocument root;
    TestGroupSet set{ &root };
    TestGroup group(&set, VoiceGroup::I);
    group.addParameter(new TestParameter<SplitPointParameter>(&group, uint16_t(1)));

    auto parameter = dynamic_cast<SplitPointParameter *>(group.findParameterByID(1));
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

ustring SplitPointParameter::getLongName() const
{
  return "Split Point";
}

ustring SplitPointParameter::getShortName() const
{
  return "Split P.";
}

static TestDriver<SplitPointParameter> driver;