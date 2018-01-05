#pragma once

#include "ExecutePlaygroundCommandJob.h"

class SelectParameterJob : public ExecutePlaygroundCommandJob
{
  private:
    typedef ExecutePlaygroundCommandJob super;

  public:
    SelectParameterJob (Job *parent, const string &cmd);
    virtual ~SelectParameterJob ();

    virtual string getURL() const override;

  private:
    SelectParameterJob (const SelectParameterJob& other) = delete;
    SelectParameterJob& operator= (const SelectParameterJob&) = delete;

    int m_ID;
};

