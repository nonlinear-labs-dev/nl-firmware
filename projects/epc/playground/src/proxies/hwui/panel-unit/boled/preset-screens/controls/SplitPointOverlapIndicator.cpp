#include <Application.h>
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>
#include "SplitPointOverlapIndicator.h"

SplitPointOverlapIndicator::SplitPointOverlapIndicator(const Rect& pos)
    : LabelRegular8(pos)
    , m_splitICP { 0 }
    , m_splitIICP { 0 }
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto split1 = eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::I });
  auto split2 = eb->findParameterByID({ C15::PID::Split_Split_Point, VoiceGroup::II });

  splitIConnection = split1->onParameterChanged(sigc::mem_fun(this, &SplitPointOverlapIndicator::onSplitIChanged));
  splitIIConnection = split2->onParameterChanged(sigc::mem_fun(this, &SplitPointOverlapIndicator::onSplitIIChanged));

  m_splitICP = split1->getControlPositionValue();
  m_splitIICP = split2->getControlPositionValue();
  setText(m_splitICP > m_splitIICP ? "!" : "");
}

SplitPointOverlapIndicator::~SplitPointOverlapIndicator()
{
  splitIConnection.disconnect();
  splitIIConnection.disconnect();
}

void SplitPointOverlapIndicator::onSplitIChanged(const Parameter* splitI)
{
  if(splitI)
  {
    m_splitICP = splitI->getControlPositionValue();
    setText(m_splitICP >= m_splitIICP ? "!" : "");
  }
}

int SplitPointOverlapIndicator::getZPriority() const
{
  return 2;
}

void SplitPointOverlapIndicator::onSplitIIChanged(const Parameter* splitII)
{
  if(splitII)
  {
    m_splitIICP = splitII->getControlPositionValue();
    setText(m_splitICP >= m_splitIICP ? "!" : "");
  }
}
