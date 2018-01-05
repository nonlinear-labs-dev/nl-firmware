#include "SetModulationAmountJob.h"

SetModulationAmountJob::SetModulationAmountJob (Job *parent, const string &cmd) :
    super (parent)
{
  m_value = parseFloat(cmd, "value", 0);
}

SetModulationAmountJob::~SetModulationAmountJob ()
{
}

string SetModulationAmountJob::getURL () const
{
  stringstream str;
  str << BASE_URL << "/presets/param-editor/set-mod-amount?amount=" << m_value;
  return str.str();
}

