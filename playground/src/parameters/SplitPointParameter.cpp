#include "SplitPointParameter.h"

#include <testing/TestDriver.h>
#include <parameters/scale-converters/KeyWithOctaveScaleConverter.h>
#include <testing/TestRootDocument.h>
#include <testing/parameter/TestGroup.h>
#include <testing/parameter/TestParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/DualVoiceGroupMasterAndSplitPointLayout.h>
#include "groups/ParameterGroup.h"
#include "proxies/hwui/HWUI.h"
#include <proxies/hwui/panel-unit/boled/parameter-screens/ParameterInfoLayout.h>

SplitPointParameter::SplitPointParameter(ParameterGroup *group, const ParameterId &id)
    : ModulateableParameter(group, id, ScaleConverter::get<KeyWithOctaveScaleConverter>(), 0.5, 60, 60)
{
}

Glib::ustring SplitPointParameter::getGroupAndParameterName() const
{
  return "Split Point";
}

DFBLayout *SplitPointParameter::createLayout(FocusAndMode focusAndMode) const
{
  switch(focusAndMode.mode)
  {
    case UIMode::Info:
      return new ParameterInfoLayout();
    case UIMode::Edit:
      return new UnmodulateableParameterEditLayout2();
    case UIMode::Select:
    default:
      return new DualVoiceGroupMasterAndSplitPointLayout();
  }
}

std::string SplitPointParameter::getDisplayValue(VoiceGroup vg) const
{
  auto converter = ScaleConverter::get<KeyWithOctaveScaleConverter>();

  if(vg == VoiceGroup::II)
    return converter->getDimension().stringize(getValue().getRawValue());
  else if(vg == VoiceGroup::I)
    return converter->getDimension().stringize(getNextStepValue(-1, {}));

  return "";
}

Glib::ustring SplitPointParameter::getDisplayString() const
{
  return getDisplayValue(Application::get().getHWUI()->getCurrentVoiceGroup());
}

void SplitPointParameter::registerTests()
{

  g_test_add_func("/SplitPointParameter/stringize", [] {
    TestRootDocument root;
    TestGroupSet set{ &root };
    TestGroup group(&set, VoiceGroup::I);
    group.addParameter(new TestParameter<SplitPointParameter>(&group, ParameterId{ 1, VoiceGroup::Global }));

    auto parameter = dynamic_cast<SplitPointParameter *>(group.findParameterByID({ 1, VoiceGroup::Global }));
    g_assert(parameter != nullptr);

    auto transScope = UNDO::Scope::startTrashTransaction();
    auto transaction = transScope->getTransaction();

    parameter->stepCPFromHwui(transaction, 1, {});
    g_assert(parameter->getDisplayValue(VoiceGroup::I) == "F#3");
    g_assert(parameter->getDisplayValue(VoiceGroup::II) == "G3");

    parameter->stepCPFromHwui(transaction, 1, {});
    g_assert(parameter->getDisplayValue(VoiceGroup::I) == "G3");
    g_assert(parameter->getDisplayValue(VoiceGroup::II) == "G#3");
  });
}

Glib::ustring SplitPointParameter::getLongName() const
{
  return "Split Point";
}

Glib::ustring SplitPointParameter::getShortName() const
{
  return "Split P.";
}

static TestDriver<SplitPointParameter> driver;
