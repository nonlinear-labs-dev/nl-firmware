#pragma once

#include "Job.h"

class ParseScriptJob : public Job
{
 private:
  typedef Job super;

 public:
  ParseScriptJob(Job* parent);
  virtual ~ParseScriptJob();

  virtual void implRun() override;

 private:
  ParseScriptJob(const ParseScriptJob& other) = delete;
  ParseScriptJob& operator=(const ParseScriptJob&) = delete;
};
