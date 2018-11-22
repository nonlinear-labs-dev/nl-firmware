#include "SetModulationSourceJob.h"

SetModulationSourceJob::SetModulationSourceJob(Job *parent, const string &cmd)
    : super(parent)
{
  m_value = parseInt(cmd, "value", 0);
}

SetModulationSourceJob::~SetModulationSourceJob()
{
}

string SetModulationSourceJob::getURL() const
{
  stringstream str;
  str << BASE_URL << "/presets/param-editor/set-mod-src?source=" << m_value;
  return str.str();
}
