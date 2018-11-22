#pragma once

#include "Job.h"

class LoadScriptJob : public Job
{
 private:
  typedef Job super;

 public:
  LoadScriptJob(Job* parent);
  virtual ~LoadScriptJob();

  void implRun() override;
  string getContent() const;

 private:
  LoadScriptJob(const LoadScriptJob& other) = delete;
  LoadScriptJob& operator=(const LoadScriptJob&) = delete;

  string m_content;
};
