#pragma once

#include <nltools/Types.h>
#include <ParameterId.h>
#include "ParameterCarousel.h"
#include <sigc++/connection.h>

class MonoParameterCarousel : public ParameterCarousel
{
 public:
  explicit MonoParameterCarousel(const Rect &rect);
  ~MonoParameterCarousel() override;
  void setup(Parameter *selectedParameter) override;

 private:
  static std::vector<ParameterId> getParameterIdsForMode(SoundType type);
  void setupMonoControls(Parameter *parameter);
  void rebuild();

  sigc::connection m_editbufferConnection;
  sigc::connection m_voiceGroupConnection;
};
