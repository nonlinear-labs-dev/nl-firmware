#include "Job.h"

Job::Job(Job *parent)
    : m_parent(parent)
    , m_lineNumber(0)
{
}

Job::~Job()
{
}

Job *Job::getRoot()
{
  return m_parent;
}

Job *Job::getParent()
{
  return m_parent;
}

void Job::setLineNumber(int i)
{
  m_lineNumber = i;
}

int Job::getLineNumber() const
{
  return m_lineNumber;
}

SoupSession *Job::getSession()
{
  static SoupSession *session = soup_session_new();
  return session;
}

void Job::add(Job *child)
{
  m_children.push_back(tChild(child));
}

void Job::onChildFinished(Job *child, bool success)
{
  g_assert(m_children.front().get() == child);

  m_finishedChildren.push_back(m_children.front());
  m_children.pop_front();

  if(success)
  {
    DEBUG("Child " << typeid(*child).name() << " succeeded.");
    run();
  }
  else
  {
    WARN("Child " << typeid(*child).name() << " failed. (Line #" << child->getLineNumber() << ")");
    m_parent->onChildFinished(this, false);
  }
}

void Job::run()
{
  DEBUG("Now running " << typeid(*this).name() << " ...");

  if(m_children.empty())
    implRun();
  else
    m_children.front()->run();
}

int Job::parseInt(const string &cmd, const string &name, int def) const
{
  auto pos = cmd.find(name + ":");

  if(pos != string::npos)
  {
    string sub = cmd.substr(pos + name.length() + 1);
    return std::stoi(sub);
  }

  return def;
}

float Job::parseFloat(const string &cmd, const string &name, float def) const
{
  auto pos = cmd.find(name + ":");

  if(pos != string::npos)
  {
    string sub = cmd.substr(pos + name.length() + 1);
    return std::stod(sub);
  }

  return def;
}
