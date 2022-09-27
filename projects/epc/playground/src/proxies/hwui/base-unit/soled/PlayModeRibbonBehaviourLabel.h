#pragma once

#include "proxies/hwui/controls/Label.h"
#include "ParameterId.h"

class Application;
class EditBuffer;
class Parameter;
class ParameterId;
class Setting;

class PlayModeRibbonBehaviourLabel : public Label
{
 private:
  typedef Label super;

 public:
  PlayModeRibbonBehaviourLabel(const ParameterId& paramID, const Rect& rect);
  ~PlayModeRibbonBehaviourLabel() override;

  PlayModeRibbonBehaviourLabel(const PlayModeRibbonBehaviourLabel& other) = delete;
  PlayModeRibbonBehaviourLabel& operator=(const PlayModeRibbonBehaviourLabel&) = delete;

 private:
  void onRibbonSelectionChanged(const Setting* s);
  void updateText(const Parameter* s);
  EditBuffer* getEditBuffer() const;

  sigc::connection m_connection;
  ParameterId m_ribbonParamID;
};
