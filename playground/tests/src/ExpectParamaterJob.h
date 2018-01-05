#pragma once

#include "Job.h"

class ExpectParamaterJob : public Job
{
  private:
    typedef Job super;

  public:
    ExpectParamaterJob (Job *parent, const string &cmd);
    virtual ~ExpectParamaterJob ();

    void implRun () override;

  private:
    ExpectParamaterJob (const ExpectParamaterJob& other) = delete;
    ExpectParamaterJob& operator= (const ExpectParamaterJob&) = delete;

    int m_ID;
    float m_value;
};

