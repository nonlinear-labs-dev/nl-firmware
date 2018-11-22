#include "SelectParameterJob.h"

SelectParameterJob::SelectParameterJob(Job *parent, const string &cmd)
    : super(parent)
{
  m_ID = parseInt(cmd, "id", 0);
}

SelectParameterJob::~SelectParameterJob()
{
}

string SelectParameterJob::getURL() const
{
  stringstream str;
  str << BASE_URL << "/presets/param-editor/select-param?id=" << m_ID;
  return str.str();
}
