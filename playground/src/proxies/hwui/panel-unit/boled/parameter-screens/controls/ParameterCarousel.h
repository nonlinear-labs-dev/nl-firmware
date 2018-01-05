#pragma once

#include "Carousel.h"

class Application;
class Parameter;
class PanelUnitParameterEditMode;

class ParameterCarousel : public Carousel
{
  private:
    typedef Carousel super;

  public:
    ParameterCarousel (const Rect &pos);
    virtual ~ParameterCarousel ();

    virtual void turn () override;

  protected:
    virtual void setup (Parameter *selectedParameter) override;

  private:
    ParameterCarousel (const ParameterCarousel &other);
    ParameterCarousel &operator= (const ParameterCarousel &);
    void setupChildControls (const std::shared_ptr<PanelUnitParameterEditMode>& edit, Parameter* selectedParameter, int button);
    void setupChildControls (Parameter* selectedParameter, const std::__cxx11::list<int>& buttonAssignments);
};

