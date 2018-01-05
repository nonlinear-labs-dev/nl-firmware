#include "ExecutePlaygroundCommandJob.h"

ExecutePlaygroundCommandJob::ExecutePlaygroundCommandJob (Job *parent) :
    super (parent)
{
}

ExecutePlaygroundCommandJob::~ExecutePlaygroundCommandJob ()
{
}

void ExecutePlaygroundCommandJob::implRun ()
{
  SoupMessage *msg = soup_message_new ("GET", getURL().c_str());
  soup_session_queue_message (getSession(), msg, (SoupSessionCallback)&ExecutePlaygroundCommandJob::messageCB, this);
}


void ExecutePlaygroundCommandJob::messageCB(SoupSession *session, SoupMessage *msg, ExecutePlaygroundCommandJob *job)
{
  job->getParent ()->onChildFinished (job, SOUP_STATUS_IS_SUCCESSFUL (msg->status_code));
}
