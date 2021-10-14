#pragma once

#include "ConditionBase.h"
#include <sigc++/connection.h>

class Parameter;

namespace DescriptiveLayouts
{
  namespace ParameterConditions
  {

    class ParameterCondition : public ConditionBase
    {
     public:
      ParameterCondition();
      ~ParameterCondition() override;

     protected:
      void onParameterSelectionChanged(const Parameter* oldParam, Parameter* newParam);
      virtual void onParameterChanged(const Parameter* param);

      sigc::connection m_paramChangedConnection;
      sigc::connection m_paramConnection;
    };

    template<typename tCondition>
    class InvertedCondition : public tCondition
    {
     public:
      bool check() const override {
        return !tCondition::check();
      }
    };

    class IsParameterModulateable : public ParameterCondition
    {
     public:
      bool check() const override;
    };

    class IsParameterUnmodulateable : public InvertedCondition<IsParameterModulateable>
    {};

    class HasNoMcSelected : public ParameterCondition
    {
     public:
      bool check() const override;
    };

    class HasMcSelected : public InvertedCondition<HasNoMcSelected>
    {
    };
  };
}
