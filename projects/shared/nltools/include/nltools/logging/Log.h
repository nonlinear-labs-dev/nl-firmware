#pragma once

#include "../StringTools.h"

namespace nltools
{
  class Log
  {
   public:
    enum Level
    {
      Debug,
      Info,
      Warning,
      Error,
      Notify,
      Silent
    };

    struct MemUsageLogger
    {
      MemUsageLogger(Level l, const char *loc);
      ~MemUsageLogger();

      int64_t getUsage() const;

      Level level;
      const char *loc;
      int64_t begin;
    };

    static void setLevel(Level l)
    {
      s_level = l;
    }

    static Level getLevel()
    {
      return s_level;
    }

    static void flushAfterEveryLog(bool f)
    {
      s_flush = f;
    }

    enum class LogMode
    {
      Plain,
      AppendNewLine,
      InsertSpaces,
      InsertSpacesAndAppendNewLine
    };

    static void memUsage(Level l, const char *location);
    static void memUsage(Level l, const char *location, size_t bytes);

    template <LogMode mode = LogMode::InsertSpacesAndAppendNewLine, typename... Args>
    static void debug(const Args &... args)
    {
      if(s_level <= Level::Debug)
        output<mode>("Debug: ", args...);
    }

    template <LogMode mode = LogMode::InsertSpacesAndAppendNewLine, typename... Args>
    static void info(const Args &... args)
    {
      if(s_level <= Level::Info)
        output<mode>("Info: ", args...);
    }

    template <LogMode mode = LogMode::InsertSpacesAndAppendNewLine, typename... Args>
    static void warning(const Args &... args)
    {
      if(s_level <= Level::Warning)
        output<mode>("Warning: ", args...);
    }

    template <LogMode mode = LogMode::InsertSpacesAndAppendNewLine, typename... Args>
    static void error(const Args &... args)
    {
      if(s_level <= Level::Error)
        output<mode>("Error: ", args...);
    }

    template <LogMode mode = LogMode::InsertSpacesAndAppendNewLine, typename... Args>
    static void notify(const Args &... args)
    {
      if(s_level <= Level::Notify)
        output<mode>("Notify: ", args...);
    }

    template <LogMode mode = LogMode::InsertSpacesAndAppendNewLine, typename... Args>
    static void printWithLevel(Level l, const Args &... args)
    {
      switch(l)
      {
        case Debug:
          if(s_level <= Level::Debug)
            output<mode>("Debug: ", args...);
          return;
        case Info:
          if(s_level <= Level::Info)
            output<mode>("Info: ", args...);
          break;
        case Warning:
          if(s_level <= Level::Warning)
            output<mode>("Warning: ", args...);
          break;
        case Error:
          if(s_level <= Level::Error)
            output<mode>("Error: ", args...);
          break;
        case Notify:
          if(s_level <= Level::Notify)
            output<mode>("Notify: ", args...);
          break;
        case Silent:
          break;
      }
    }

    static void flush();
    static void print(const std::string &str);
    static void printNewLine();

    template <LogMode mode = LogMode::InsertSpacesAndAppendNewLine, typename... Args>
    static void output(const Args &... args)
    {
      constexpr auto addNewLine = mode == LogMode::AppendNewLine || mode == LogMode::InsertSpacesAndAppendNewLine;
      constexpr auto insertSpaces = mode == LogMode::InsertSpaces || mode == LogMode::InsertSpacesAndAppendNewLine;

      if(insertSpaces)
        print(nltools::string::concatWithDelimiter(' ', args...));
      else
        print(nltools::string::concat(args...));

      if(addNewLine)
        printNewLine();

      if(s_flush)
        flush();
    }

    template <typename... tArgs> static void throwException(const tArgs &... args)
    {
      auto str = nltools::string::concat(args...);
      error(str);
      throw std::runtime_error(str);
    }

    static void printbacktrace(Level level, int maxFrames = 500);

   private:
    static Level s_level;
    static bool s_flush;
  };
}
