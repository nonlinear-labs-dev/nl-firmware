#include "SpawnPlaygroundJob.h"

SpawnPlaygroundJob::SpawnPlaygroundJob (Job *parent) :
    super (parent)
{
}

SpawnPlaygroundJob::~SpawnPlaygroundJob ()
{
}

void SpawnPlaygroundJob::implRun ()
{
  Glib::spawn_command_line_async ("../playground");
  getParent()->onChildFinished(this, true);
}

