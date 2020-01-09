#pragma once

#include <proxies/hwui/controls/LabelRegular8.h>
#include <sigc++/connection.h>

class Parameter;

class SplitPointLabel : public LabelRegular8
{
 public:
  explicit SplitPointLabel(const Rect& r);

 protected:
  void onParameterChanged(const Parameter* parameter);
  sigc::connection m_connection;
};
