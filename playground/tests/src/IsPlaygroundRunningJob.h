#pragma once

#include "Job.h"

class IsPlaygroundRunningJob : public Job
{
 private:
  typedef Job super;

 public:
  IsPlaygroundRunningJob(Job* parent);
  virtual ~IsPlaygroundRunningJob();

  void implRun() override;

 private:
  IsPlaygroundRunningJob(const IsPlaygroundRunningJob& other) = delete;
  IsPlaygroundRunningJob& operator=(const IsPlaygroundRunningJob&) = delete;
};
