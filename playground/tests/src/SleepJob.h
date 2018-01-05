#pragma once

#include "Job.h"

class SleepJob : public Job
{
  private:
    typedef Job super;

  public:
    SleepJob (Job *parent, const string &cmd);
    virtual ~SleepJob ();

    void implRun() override;

  private:
    SleepJob (const SleepJob& other) = delete;
    SleepJob& operator= (const SleepJob&) = delete;

    int m_seconds;
};

