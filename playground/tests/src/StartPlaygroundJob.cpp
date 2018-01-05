#include "StartPlaygroundJob.h"
#include "IsPlaygroundRunningJob.h"
#include "SpawnPlaygroundJob.h"

StartPlaygroundJob::StartPlaygroundJob (Job *parent) :
    super (parent)
{
  add<IsPlaygroundRunningJob> ();
}

StartPlaygroundJob::~StartPlaygroundJob ()
{
}

void StartPlaygroundJob::onChildFinished (Job *child, bool success)
{
  if (dynamic_cast<IsPlaygroundRunningJob*> (child))
  {
    if(!success)
      add<SpawnPlaygroundJob>();

    success = true;
  }

  super::onChildFinished(child, success);
}

void StartPlaygroundJob::implRun ()
{
  getParent()->onChildFinished(this, true);
}
