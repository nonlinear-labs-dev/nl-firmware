#pragma once

#include <parameters/ModulateableParameter.h>

class ScaleParameter : public ModulateableParameter
{
  typedef ModulateableParameter super;

 public:
  ScaleParameter(ParameterGroup *group, const ParameterId& id);

  void writeDocProperties(Writer &writer, tUpdateID knownRevision) const override;
  Glib::ustring getLongName() const override;
  size_t getHash() const override;
};

class BaseScaleParameter : public Parameter
{
  typedef Parameter super;

 public:
  BaseScaleParameter(ParameterGroup* group, const ParameterId& id);
  void writeDocProperties(Writer& writer, tUpdateID knownRevision) const override;
};
