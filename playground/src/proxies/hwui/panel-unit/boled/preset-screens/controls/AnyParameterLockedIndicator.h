#pragma once

#include <boost/logic/tribool.hpp>
#include <tools/DelayedJob.h>
#include "InvertedLabel.h"

class Application;
class Parameter;

class AnyParameterLockedIndicator : public InvertedLabel
{
  private:
    typedef InvertedLabel super;

  public:
    AnyParameterLockedIndicator (const Rect &pos);
    virtual ~AnyParameterLockedIndicator ();

    bool isVisible () const override;

  private:
    void calcHasLocks();
    void update ();

    DelayedJob m_calcHasLocks;
    boost::tribool m_hasLocks = boost::tribool::indeterminate_value;
};

