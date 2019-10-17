#pragma once
#include "ParameterCarousel.h"
#include <parameters/Parameter.h>

class DualParameterScreenCarousel : public ParameterCarousel
{
 public:
  DualParameterScreenCarousel(const Rect& r);

 private:
  void rebuild();

protected:
  void setup(Parameter *selectedParameter) override;
  void setupMasterParameters(const std::vector<Parameter::ID> &parameters);

private:
  sigc::connection m_editbufferConnection;
};