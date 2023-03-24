#include "ChangedParameterIndicator.h"
#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/FrameBuffer.h>

ChangedParameterIndicator::ChangedParameterIndicator(const Rect& pos)
    : Label(StringAndSuffix { "" }, pos)
    , m_worker(Glib::MainContext::get_default(), std::chrono::milliseconds { 50 })
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  eb->onChange(sigc::mem_fun(this, &ChangedParameterIndicator::update));
}

void ChangedParameterIndicator::update()
{
  m_worker.doTask([this]() {
    auto eb = Application::get().getPresetManager()->getEditBuffer();
    auto partGroupI = eb->getParameterGroupByID(GroupId("Part", VoiceGroup::I));
    auto partGroupII = eb->getParameterGroupByID(GroupId("Part", VoiceGroup::II));
    auto monoGroupI = eb->getParameterGroupByID(GroupId("Mono", VoiceGroup::I));
    auto monoGroupII = eb->getParameterGroupByID(GroupId("Mono", VoiceGroup::II));
    auto unisonGroupI = eb->getParameterGroupByID(GroupId("Unison", VoiceGroup::I));
    auto unisonGroupII = eb->getParameterGroupByID(GroupId("Unison", VoiceGroup::II));
    auto masterGroup = eb->getParameterGroupByID(GroupId("Master", VoiceGroup::Global));
    auto scaleGroup = eb->getParameterGroupByID(GroupId("Scale", VoiceGroup::Global));

    auto partChanged = false;
    auto monoChanged = false;
    auto unisonChanged = false;
    auto masterChanged = masterGroup->isAnyParameterChanged();
    auto scaleChanged = scaleGroup->isAnyParameterChanged();

    switch(eb->getType())
    {
      case SoundType::Single:
        partChanged = false;
        monoChanged = monoGroupI->isAnyParameterChanged();
        unisonChanged = unisonGroupI->isAnyParameterChanged();
        break;
      case SoundType::Layer:
        partChanged = partGroupI->isAnyParameterChanged() || partGroupII->isAnyParameterChanged();
        monoChanged = monoGroupI->isAnyParameterChanged();
        unisonChanged = unisonGroupI->isAnyParameterChanged();
        break;
      case SoundType::Split:
        partChanged = partGroupI->isAnyParameterChanged() || partGroupII->isAnyParameterChanged();
        monoChanged = monoGroupI->isAnyParameterChanged() || monoGroupII->isAnyParameterChanged();
        unisonChanged = unisonGroupI->isAnyParameterChanged() || unisonGroupII->isAnyParameterChanged();
        break;
      default:
        break;
    }

    std::stringstream ss;
    ss << (partChanged ? "P" : "") << (monoChanged ? "\uE040" : "") << (unisonChanged ? "\uE041" : "")
       << (masterChanged ? "M" : "") << (scaleChanged ? "S" : "");
    setText({ ss.str(), 0 });
  });
}

int ChangedParameterIndicator::getZPriority() const
{
  if(isVisible() && !getText().text.empty())
    return 1;
  else
    return -1;
}
