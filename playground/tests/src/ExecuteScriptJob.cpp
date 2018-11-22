#include "ExecuteScriptJob.h"

ExecuteScriptJob::ExecuteScriptJob(Job *parent)
    : super(parent)
{
}

ExecuteScriptJob::~ExecuteScriptJob()
{
}

void ExecuteScriptJob::implRun()
{
  getParent()->onChildFinished(this, true);
}
