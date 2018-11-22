#include "SetDefaultsJob.h"

SetDefaultsJob::SetDefaultsJob(Job *parent)
    : super(parent)
{
}

SetDefaultsJob::~SetDefaultsJob()
{
}

string SetDefaultsJob::getURL() const
{
  return URL("/presets/param-editor/reset");
}
