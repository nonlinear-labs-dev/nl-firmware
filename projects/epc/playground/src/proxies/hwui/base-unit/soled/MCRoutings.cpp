#include "MCRoutings.h"
#include "Application.h"
#include "presets/PresetManager.h"
#include "presets/EditBuffer.h"

#include "parameters/ModulationRoutingParameter.h"
#include "parameters/PhysicalControlParameter.h"
#include "device-settings/SelectedRibbonsSetting.h"
#include <groups/HardwareSourcesGroup.h>
#include <proxies/hwui/HWUI.h>
#include <math.h>
#include <proxies/hwui/FrameBuffer.h>

MCRoutings::MCRoutings(const ParameterId &ribbonParamID, const Rect &pos)
    : super(pos)
    , m_ribbonParamID(ribbonParamID)
{
  connectToRoutingParameters();

  Application::get().getSettings()->getSetting<SelectedRibbonsSetting>()->onChange(sigc::mem_fun(this, &MCRoutings::onRibbonSelectionChanged));
}

MCRoutings::~MCRoutings()
{
}

void MCRoutings::connectToRoutingParameters()
{
  auto routingParams = getRoutingParameters();

  for(auto routingParam : routingParams)
    routingParam->onParameterChanged(mem_fun(this, &MCRoutings::onRoutingParameterChanged));
}

bool MCRoutings::redraw(FrameBuffer &fb)
{
  super::redraw(fb);

  int numAmounts = 0;
  auto params = getRoutingParameters();

  for(auto param : params)
    if(abs(param->getControlPositionValue()) > 0)
      numAmounts++;

  if(numAmounts > 1)
  {
    fb.setColor(FrameBufferColors::C128);
    drawRoutingParameters(fb, params);
  }

  return true;
}

void MCRoutings::onRibbonSelectionChanged(const Setting* s)
{
  if(auto selected = dynamic_cast<const SelectedRibbonsSetting*>(s))
  {
    auto isOneAndTwo = selected->get() == SelectedRibbons::Ribbon1_2;

    if(isOneAndTwo)
    {
      if(m_ribbonParamID.getNumber() == C15::PID::Ribbon_3)
        m_ribbonParamID = { C15::PID::Ribbon_1, VoiceGroup::Global };
      else if(m_ribbonParamID.getNumber() == C15::PID::Ribbon_4)
        m_ribbonParamID = { C15::PID::Ribbon_2, VoiceGroup::Global };
    }
    else
    {
      if(m_ribbonParamID.getNumber() == C15::PID::Ribbon_1)
        m_ribbonParamID = { C15::PID::Ribbon_3, VoiceGroup::Global };
      else
        m_ribbonParamID = { C15::PID::Ribbon_4, VoiceGroup::Global };
    }
  }
  setDirty();
}

EditBuffer *MCRoutings::getEditBuffer() const
{
  return Application::get().getPresetManager()->getEditBuffer();
}

MacroControlMappingGroup::tModRoutingParams MCRoutings::getRoutingParameters()
{
  auto eb = getEditBuffer();
  auto cs = dynamic_cast<HardwareSourcesGroup *>(eb->getParameterGroupByID({ "CS", VoiceGroup::Global }));
  auto routers = dynamic_cast<MacroControlMappingGroup *>(eb->getParameterGroupByID({ "MCM", VoiceGroup::Global }));
  auto ribbonParam = dynamic_cast<PhysicalControlParameter *>(cs->getParameterByID(m_ribbonParamID));
  auto routingParams = routers->getModulationRoutingParametersFor(ribbonParam);
  return routingParams;
}

void MCRoutings::onRoutingParameterChanged(const Parameter *p)
{
  setDirty();
}

void MCRoutings::drawRoutingParameters(FrameBuffer &fb, MacroControlMappingGroup::tModRoutingParams &params)
{
  int inc = getPosition().getHeight() / 4;
  int pos = getPosition().getTop();

  for(auto param : params)
  {
    drawRoutingParameter(fb, param, pos);
    pos += inc;
  }
}

void MCRoutings::drawRoutingParameter(FrameBuffer &fb, MacroControlMappingGroup::tModRoutingParam param, int yPos)
{
  auto pos = getPosition();
  auto val = std::ceil(abs(param->getControlPositionValue()) * 5);
  int height = getPosition().getHeight() / 4;

  yPos += height / 2;
  fb.setPixel(pos.getX(), yPos);

  if(val > 0)
    fb.drawHorizontalLine(pos.getX() + 2, yPos, static_cast<int>(val - 1));
}
