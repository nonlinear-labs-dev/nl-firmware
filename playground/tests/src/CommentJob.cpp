#include "CommentJob.h"

CommentJob::CommentJob(Job *parent, const string &cmd)
    : super(parent)
    , m_cmd(cmd)
{
}

CommentJob::~CommentJob()
{
}

void CommentJob::implRun()
{
  cout << m_cmd << endl;
  getParent()->onChildFinished(this, true);
}
