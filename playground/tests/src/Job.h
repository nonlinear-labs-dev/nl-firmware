#pragma once

#include "inc.h"
#include <list>

#define BASE_URL "http://localhost:8080"
#define URL(path) BASE_URL path

#define TRACE(arg) std::cout << arg << std::endl
#define DEBUG(arg)
//#define DEBUG( arg ) TRACE(arg)
#define WARN(arg) TRACE(arg)

class Job
{
 private:
 public:
  typedef std::shared_ptr<Job> tChild;
  typedef std::list<tChild> tChildren;

  Job(Job *parent);
  virtual ~Job();

  static SoupSession *getSession();

  template <typename T> void add()
  {
    add(new T(this));
  }

  template <typename T> std::shared_ptr<T> findChild()
  {
    for(auto c : m_children)
    {
      if(auto t = std::dynamic_pointer_cast<T>(c))
        return t;

      if(auto t = c->findChild<T>())
        return t;
    }

    for(auto c : m_finishedChildren)
    {
      if(auto t = std::dynamic_pointer_cast<T>(c))
        return t;

      if(auto t = c->findChild<T>())
        return t;
    }

    return nullptr;
  }

  virtual Job *getRoot();
  Job *getParent();
  void setLineNumber(int i);
  int getLineNumber() const;

  void add(Job *child);
  virtual void onChildFinished(Job *child, bool success);
  void run();

 protected:
  virtual void implRun() = 0;
  int parseInt(const string &cmd, const string &name, int def) const;
  float parseFloat(const string &cmd, const string &name, float def) const;

 private:
  void runChild();

  Job *m_parent = nullptr;
  int m_lineNumber;

  tChildren m_children;
  tChildren m_finishedChildren;
};
