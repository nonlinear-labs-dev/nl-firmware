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
  std::cout << m_cmd << std::endl;
  getParent()->onChildFinished(this, true);
}
