#pragma once

#include "ExecutePlaygroundCommandJob.h"

class SetModulationSourceJob : public ExecutePlaygroundCommandJob
{
 private:
  typedef ExecutePlaygroundCommandJob super;

 public:
  SetModulationSourceJob(Job* parent, const string& cmd);
  virtual ~SetModulationSourceJob();

  virtual string getURL() const override;

 private:
  SetModulationSourceJob(const SetModulationSourceJob& other) = delete;
  SetModulationSourceJob& operator=(const SetModulationSourceJob&) = delete;

  int m_value;
};
