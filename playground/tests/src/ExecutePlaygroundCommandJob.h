#pragma once

#include "Job.h"

class ExecutePlaygroundCommandJob : public Job
{
  private:
    typedef Job super;

  public:
    ExecutePlaygroundCommandJob (Job *parent);
    virtual ~ExecutePlaygroundCommandJob ();

    void implRun () override;

    virtual string getURL() const = 0;

  private:
    ExecutePlaygroundCommandJob (const ExecutePlaygroundCommandJob& other) = delete;
    ExecutePlaygroundCommandJob& operator= (const ExecutePlaygroundCommandJob&) = delete;

    static void messageCB(SoupSession *session, SoupMessage *msg, ExecutePlaygroundCommandJob *job);

};

