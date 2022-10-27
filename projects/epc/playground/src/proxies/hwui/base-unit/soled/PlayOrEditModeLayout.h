#pragma once

#include "proxies/hwui/Layout.h"

class Label;
class Setting;
class Parameter;

class PlayOrEditModeLayout : public Layout
{
 private:
  typedef Layout super;

 public:
  PlayOrEditModeLayout();
  ~PlayOrEditModeLayout() override;

 protected:
  int getBehaviourLeft() const;
  void onParameterSelectionChanged(const Parameter* old, Parameter* newP);

 private:
  PlayOrEditModeLayout(const PlayOrEditModeLayout& other);
  PlayOrEditModeLayout& operator=(const PlayOrEditModeLayout&);
};
