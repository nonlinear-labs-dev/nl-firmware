#pragma once

#include <nltools/Types.h>
#include "ParameterCarousel.h"

class MonoParameterCarousel : public ParameterCarousel {
public:
  explicit MonoParameterCarousel(const Rect& rect);
  ~MonoParameterCarousel() override;
  void setup(Parameter *selectedParameter) override;
  static VoiceGroup getVoiceGroup();

  void setupMonoControls(Parameter *parameter);
  void rebuild();
private:
  sigc::connection m_editbufferConnection;
};
