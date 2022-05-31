#pragma once

#include <string>
#include <vector>
#include <functional>
#include <list>
#include <memory>

class AsyncCommandLine;

class SerializedAsyncCommands
{
 public:
  SerializedAsyncCommands();
  ~SerializedAsyncCommands();

  using Command = std::vector<std::string>;
  using SuccessCB = std::function<void(const std::string &)>;
  using ErrorCB = std::function<void(const std::string &)>;

  void schedule(const Command &command, const SuccessCB &onSuccess, const ErrorCB &onError);

 private:
  void doCommand();

  struct Record
  {
    Command cmd;
    SuccessCB success;
    ErrorCB error;
  };

  std::list<Record> m_records;
  std::unique_ptr<AsyncCommandLine> m_currentCommand;
};
