#include <Application.h>
#include <parameters/Parameter.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameters/ModulateableParameter.h>
#include "ConditionRegistry.h"
#include "Conditions/ParameterConditions.h"
#include "Conditions/SoundConditions.h"
#include <device-settings/LayoutMode.h>
#include <proxies/hwui/descriptive-layouts/Conditions/HWUIConditions.h>

ConditionRegistry::tCondition ConditionRegistry::getLambda(const std::string& key)
{
 #warning"adlerauge"
  return m_theConditionMap.at(key).get();
}

ConditionRegistry& ConditionRegistry::get()
{
  static ConditionRegistry theRegistry;
  return theRegistry;
}

ConditionRegistry::ConditionRegistry()
{
  using namespace DescriptiveLayouts;
  m_theConditionMap["isParameterModulateable"] = std::make_unique<ParameterConditions::IsParameterModulateable>();
  m_theConditionMap["isParameterUnmodulateable"] = std::make_unique<ParameterConditions::IsParameterUnmodulateable>();
  m_theConditionMap["hasNoMcSelected"] = std::make_unique<ParameterConditions::HasNoMcSelected>();
  m_theConditionMap["hasMcSelected"] = std::make_unique<ParameterConditions::HasMcSelected>();
  m_theConditionMap["isSingleSound"] = std::make_unique<SoundConditions::IsSingleSound>();
  m_theConditionMap["isSplitSound"] = std::make_unique<SoundConditions::IsSplitSound>();
  m_theConditionMap["isLayerSound"] = std::make_unique<SoundConditions::IsLayerSound>();
  m_theConditionMap["isFineActive"] = std::make_unique<HWUIConditions::IsFineActive>();
}

sigc::connection ConditionRegistry::onChange(const std::function<void()>& cb)
{
  return m_signal.connect(cb);
}

void ConditionRegistry::onConditionChanged()
{
  if(Application::get().getSettings()->getSetting<LayoutMode>()->get() != LayoutVersionMode::Old)
  {
    m_signal.send();
  }
}