#pragma once

#include "ExecutePlaygroundCommandJob.h"

class SetParameterJob : public ExecutePlaygroundCommandJob
{
  private:
    typedef ExecutePlaygroundCommandJob super;

  public:
    SetParameterJob (Job *parent, const string &cmd);
    virtual ~SetParameterJob ();

    virtual string getURL() const override;

  private:
    SetParameterJob (const SetParameterJob& other) = delete;
    SetParameterJob& operator= (const SetParameterJob&) = delete;

    int m_ID;
    float m_value;
};

