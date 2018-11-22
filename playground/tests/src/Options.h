#pragma once

#include "Tests.h"

class Options
{
 public:
  Options(int &argc, char **&argv);
  virtual ~Options();

  string getScript() const;
  void showHelp();

 private:
  bool setScriptFile(const string &optionName, const string &file, bool hasValue);
  void showHelp(OptionContext ctx);

  OptionContext m_ctx;
  string m_script;
};
