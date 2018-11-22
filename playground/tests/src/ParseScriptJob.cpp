#include "ParseScriptJob.h"
#include "LoadScriptJob.h"
#include "ExecuteScriptJob.h"
#include "StartPlaygroundJob.h"
#include "SleepJob.h"
#include "SetDefaultsJob.h"
#include "SetParameterJob.h"
#include "SetModulationAmountJob.h"
#include "SetModulationSourceJob.h"
#include "ExpectParamaterJob.h"
#include "SelectParameterJob.h"
#include "CommentJob.h"

#include <boost/algorithm/string/trim.hpp>

ParseScriptJob::ParseScriptJob(Job* parent)
    : super(parent)
{
}

ParseScriptJob::~ParseScriptJob()
{
}

static string parseCommand(const string& line)
{
  size_t spacePos = line.find(' ');

  if(spacePos == string::npos)
    return line;

  return line.substr(0, spacePos);
}

static Job* createJob(const string& line, Job* parent)
{
  typedef function<Job*(Job*, const string&)> tConstructor;

  static map<string, tConstructor> knownCommands = {
    { "start-playground", [](Job* parent, const string& cmd) { return new StartPlaygroundJob(parent); } },
    { "set-defaults", [](Job* parent, const string& cmd) { return new SetDefaultsJob(parent); } },
    { "set-parameter", [](Job* parent, const string& cmd) { return new SetParameterJob(parent, cmd); } },
    { "set-modulation-source", [](Job* parent, const string& cmd) { return new SetModulationSourceJob(parent, cmd); } },
    { "set-modulation-amount", [](Job* parent, const string& cmd) { return new SetModulationAmountJob(parent, cmd); } },
    { "expect-parameter", [](Job* parent, const string& cmd) { return new ExpectParamaterJob(parent, cmd); } },
    { "sleep", [](Job* parent, const string& cmd) { return new SleepJob(parent, cmd); } },
    { "select-parameter", [](Job* parent, const string& cmd) { return new SelectParameterJob(parent, cmd); } },
    { "//", [](Job* parent, const string& cmd) { return new CommentJob(parent, cmd); } }
  };

  auto it = knownCommands.find(parseCommand(line));

  if(it != knownCommands.end())
    return it->second(parent, line);

  WARN("Command '" << line << "' could not be parsed.");

  return nullptr;
}

void ParseScriptJob::implRun()
{
  bool success = false;
  string script = getRoot()->findChild<LoadScriptJob>()->getContent();
  auto exe = getRoot()->findChild<ExecuteScriptJob>();

  stringstream str(script);
  string line;
  int lineNumber = 1;

  while(getline(str, line))
  {
    boost::trim(line);

    if(!line.empty())
    {
      if(auto job = createJob(line, exe.get()))
      {
        job->setLineNumber(lineNumber);
        exe->add(job);
        success = true;
      }
    }

    lineNumber++;
  }

  getParent()->onChildFinished(this, success);
}
