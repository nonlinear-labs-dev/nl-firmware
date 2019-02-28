#include "CommandLineInterface.h"

#pragma once

class C15Synth;

class C15_CLI : public CommandLineInterface
{
 public:
  C15_CLI(C15Synth *synth);

 protected:
  void processLine(const std::string &line) override;

 private:
  C15Synth *m_synth;
  std::unordered_map<std::string, std::function<void()>> m_commands;
};
