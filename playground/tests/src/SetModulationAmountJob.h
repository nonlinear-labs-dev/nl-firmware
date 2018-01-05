#pragma once

#include "ExecutePlaygroundCommandJob.h"

class SetModulationAmountJob : public ExecutePlaygroundCommandJob
{
  private:
    typedef ExecutePlaygroundCommandJob super;

  public:
    SetModulationAmountJob (Job *parent, const string &cmd);
    virtual ~SetModulationAmountJob ();

    virtual string getURL() const override;

  private:
    SetModulationAmountJob (const SetModulationAmountJob& other) = delete;
    SetModulationAmountJob& operator= (const SetModulationAmountJob&) = delete;

    float m_value;
};

