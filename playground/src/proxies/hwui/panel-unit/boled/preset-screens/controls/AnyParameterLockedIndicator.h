#pragma once

#include <tools/DelayedJob.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/LockedIndicator.h>
#include "InvertedLabel.h"

class Application;
class Parameter;

class AnyParameterLockedIndicator : public LockedIndicator
{
  public:
    AnyParameterLockedIndicator (const Rect &pos);
    virtual ~AnyParameterLockedIndicator () = default;

  private:
    void onParameterSelected(Parameter * newOne);
    void onParameterGroupChanged();


    sigc::connection m_groupConnection;

};

