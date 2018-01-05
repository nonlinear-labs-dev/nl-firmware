#pragma once

#include "ExecutePlaygroundCommandJob.h"

class SetDefaultsJob : public ExecutePlaygroundCommandJob
{
  private:
    typedef ExecutePlaygroundCommandJob super;

  public:
    SetDefaultsJob (Job *parent);
    virtual ~SetDefaultsJob ();

    virtual string getURL() const override;

  private:
    SetDefaultsJob (const SetDefaultsJob& other) = delete;
    SetDefaultsJob& operator= (const SetDefaultsJob&) = delete;
};

