#pragma once

#include "Job.h"

class ExecuteScriptJob : public Job
{
  private:
    typedef Job super;

  public:
    ExecuteScriptJob (Job *parent);
    virtual ~ExecuteScriptJob ();

    void implRun() override;

  private:
    ExecuteScriptJob (const ExecuteScriptJob& other) = delete;
    ExecuteScriptJob& operator= (const ExecuteScriptJob&) = delete;
};

