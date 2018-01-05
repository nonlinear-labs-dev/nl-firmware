#pragma once

#include "Carousel.h"

class Application;
class Parameter;

class ModulationRoutersCarousel : public Carousel
{
  private:
    typedef Carousel super;

  public:
    ModulationRoutersCarousel (const Rect &pos);
    virtual ~ModulationRoutersCarousel ();

    virtual void turn () override;

  protected:
    virtual void setup (Parameter *selectedParameter) override;

  private:
    ModulationRoutersCarousel (const ModulationRoutersCarousel &other);
    ModulationRoutersCarousel &operator= (const ModulationRoutersCarousel &);
};

