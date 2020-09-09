#pragma once

#include <proxies/hwui/controls/ButtonMenu.h>
#include "Carousel.h"

class Application;
class Parameter;

class ModulationRoutersCarousel : public Carousel
{
 private:
  typedef Carousel super;

 public:
  explicit ModulationRoutersCarousel(const Rect &pos);
  ~ModulationRoutersCarousel() override;

  void turn() override;
  void antiTurn() override;

 protected:
 public:
  void setHighlight(bool isHighlight) override;
 protected:
  void setup(Parameter *selectedParameter) override;

 private:
  ModulationRoutersCarousel(const ModulationRoutersCarousel &other);
  ModulationRoutersCarousel &operator=(const ModulationRoutersCarousel &);
};
