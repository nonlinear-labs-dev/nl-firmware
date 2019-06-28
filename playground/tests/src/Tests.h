#pragma once

#include "Job.h"

class Options;

class Tests : public Job
{
 private:
  typedef Job super;

 public:
  Tests(int numArgs, char **argv);
  virtual ~Tests();

  void start();
  void quit();

  void implRun() override;
  void onChildFinished(Job *child, bool success) override;

  virtual Job *getRoot() override;
  std::shared_ptr<Options> getOptions();

 private:
  Tests(const Tests &other) = delete;
  Tests &operator=(const Tests &) = delete;

  void startTest();

  std::shared_ptr<Options> m_options;
  RefPtr<MainLoop> m_theMainLoop;
  std::unique_ptr<Job> m_test;
};
