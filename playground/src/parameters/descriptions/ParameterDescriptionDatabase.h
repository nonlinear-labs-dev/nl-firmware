#pragma once

#include "playground.h"
#include "ParameterId.h"

class Application;

class ParameterDescriptionDatabase
{
 public:
  static ParameterDescriptionDatabase &get();

  connection load(ParameterId paramID, slot<void, const Glib::ustring &>);

 private:
  ParameterDescriptionDatabase();

  class Job;
  typedef std::shared_ptr<Job> tJob;
  std::map<ParameterId, tJob> m_jobs;
};
