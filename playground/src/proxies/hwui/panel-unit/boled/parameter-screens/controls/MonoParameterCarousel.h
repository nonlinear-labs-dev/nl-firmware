#pragma once

#include <parameters/mono-mode-parameters/MonoParameter.h>
#include "ParameterCarousel.h"

class MonoParameterCarousel : public ParameterCarousel {
public:
  explicit MonoParameterCarousel(const Rect& rect);
  ~MonoParameterCarousel() override;
  void setup(Parameter *selectedParameter) override;
  static bool monoParametersActive();
  static VoiceGroup getVoiceGroup();

  void setupMonoControls(MonoParameter *parameter);
  void rebuild();
private:
  sigc::connection m_editbufferConnection;
};
