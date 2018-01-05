#pragma once

#include "Job.h"

class CommentJob : public Job
{
  private:
    typedef Job super;

  public:
    CommentJob (Job *parent, const string &cmd);
    virtual ~CommentJob ();

    void implRun () override;

  private:
    CommentJob (const CommentJob& other) = delete;
    CommentJob& operator= (const CommentJob&) = delete;

    string m_cmd;
};

