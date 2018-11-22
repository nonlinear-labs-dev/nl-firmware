#pragma once

#include "Job.h"

class SpawnPlaygroundJob : public Job
{
 private:
  typedef Job super;

 public:
  SpawnPlaygroundJob(Job* parent);
  virtual ~SpawnPlaygroundJob();

  void implRun() override;

 private:
  SpawnPlaygroundJob(const SpawnPlaygroundJob& other) = delete;
  SpawnPlaygroundJob& operator=(const SpawnPlaygroundJob&) = delete;
};
