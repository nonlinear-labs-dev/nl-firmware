#pragma once

#include "ParameterEvent.h"

namespace DescriptiveLayouts
{
  class CurrentMacroControlAssignment : public ParameterEvent<bool>
  {
   public:
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class CurrentMacroControlSymbol : public ParameterEvent<DisplayString>
  {
   public:
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class CurrentMacroControlAmount : public ParameterEvent<DisplayString>
  {
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class CurrentMacroControlPositionText : public ParameterEvent<DisplayString>
  {
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class CurrentModParamModRangeEventSource
      : public ParameterEvent<std::pair<tControlPositionValue, tControlPositionValue>>
  {
   public:
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class CurrentMacroControlPosition : public ParameterEvent<tControlPositionValue>
  {
   public:
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class MCPositionChanged : public ParameterEvent<bool>
  {
   public:
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class MCSelectionChanged : public ParameterEvent<bool>
  {
   public:
    void onSelectedParameterChanged(const Parameter *p) override;
  };

  class MCAmountChanged : public ParameterEvent<bool>
  {
   public:
    void onSelectedParameterChanged(const Parameter *p) override;
  };
}
