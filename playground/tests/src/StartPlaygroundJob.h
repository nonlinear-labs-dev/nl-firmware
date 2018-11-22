#pragma once

#include "Job.h"

class StartPlaygroundJob : public Job
{
 private:
  typedef Job super;

 public:
  StartPlaygroundJob(Job* parent);
  virtual ~StartPlaygroundJob();

  void onChildFinished(Job* child, bool success) override;
  void implRun() override;

 private:
  StartPlaygroundJob(const StartPlaygroundJob& other) = delete;
  StartPlaygroundJob& operator=(const StartPlaygroundJob&) = delete;
};
