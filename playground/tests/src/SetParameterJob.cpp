#include "SetParameterJob.h"

SetParameterJob::SetParameterJob(Job *parent, const string &cmd)
    : super(parent)
{
  m_ID = parseInt(cmd, "id", 0);
  m_value = parseFloat(cmd, "value", 0.0f);
}

SetParameterJob::~SetParameterJob()
{
}

string SetParameterJob::getURL() const
{
  stringstream str;
  str << BASE_URL << "/presets/param-editor/set-params?" << m_ID << "=" << m_value;
  DEBUG("Set parameter " << m_ID << " to " << m_value);
  return str.str();
}
