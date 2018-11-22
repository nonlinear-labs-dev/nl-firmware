#include "SleepJob.h"

SleepJob::SleepJob(Job *parent, const string &cmd)
    : super(parent)
{
  m_seconds = parseInt(cmd, "seconds", 0);
}

SleepJob::~SleepJob()
{
}

void SleepJob::implRun()
{
  MainContext::get_default()->signal_timeout().connect_seconds_once(
      sigc::bind(mem_fun(getParent(), &Job::onChildFinished), this, true), m_seconds);
}
