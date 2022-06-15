#include "ChangedParameterIndicator.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>

ChangedParameterIndicator::ChangedParameterIndicator(const Rect& pos)
    : Label(StringAndSuffix{""}, pos)
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onChange(sigc::mem_fun(this, &ChangedParameterIndicator::update));
}

void ChangedParameterIndicator::update()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto partGroupI = eb->getParameterGroupByID(GroupId("Part", VoiceGroup::I));
  auto partGroupII = eb->getParameterGroupByID(GroupId("Part", VoiceGroup::II));
  auto monoGroupI = eb->getParameterGroupByID(GroupId("Mono", VoiceGroup::I));
  auto monoGroupII = eb->getParameterGroupByID(GroupId("Mono", VoiceGroup::II));
  auto unisonGroupI = eb->getParameterGroupByID(GroupId("Unison", VoiceGroup::I));
  auto unisonGroupII = eb->getParameterGroupByID(GroupId("Unison", VoiceGroup::II));
  auto masterGroup = eb->getParameterGroupByID(GroupId("Master", VoiceGroup::Global));
  auto scaleGroup = eb->getParameterGroupByID(GroupId("Scale", VoiceGroup::Global));

  auto partChanged = partGroupI->isAnyParameterChanged() || partGroupII->isAnyParameterChanged();
  auto monoChanged = monoGroupI->isAnyParameterChanged() || monoGroupII->isAnyParameterChanged();
  auto unisonChanged = unisonGroupI->isAnyParameterChanged() || unisonGroupII->isAnyParameterChanged();
  auto masterChanged = masterGroup->isAnyParameterChanged();
  auto scaleChanged = scaleGroup->isAnyParameterChanged();

  std::stringstream ss;
  ss << (partChanged ? "P" : "") << (monoChanged ? "m" : "") << (unisonChanged ? "U" : "") << (masterChanged ? "M" : "") << (scaleChanged ? "S" : "");
  setText({ss.str(), 0});
}

int ChangedParameterIndicator::getZPriority() const
{
  return 1;
}
