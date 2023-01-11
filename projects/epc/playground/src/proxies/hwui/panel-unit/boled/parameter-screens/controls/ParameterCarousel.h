#pragma once

#include <proxies/hwui/buttons.h>
#include "Carousel.h"
#include "MiniParameter.h"
#include <nltools/Types.h>

class Application;
class Parameter;
class PanelUnitParameterEditMode;

class ParameterCarousel : public Carousel
{
 private:
  typedef Carousel super;

 public:
  explicit ParameterCarousel(const Rect &pos);
  ~ParameterCarousel() override;

  ParameterCarousel(const ParameterCarousel &other) = delete;
  ParameterCarousel &operator=(const ParameterCarousel &) = delete;

  void turn() override;
  void antiTurn() override;

 protected:
  void setup(Parameter *selectedParameter) override;

  std::vector<int> m_currentCarouselContentIDs;

 private:
  using tPanelParameterEditMode = std::shared_ptr<PanelUnitParameterEditMode>;
  void setupChildControls(const tPanelParameterEditMode &edit, Parameter *selectedParameter, Buttons button);
  void setupChildControls(Parameter *selectedParameter, const std::__cxx11::list<int> &buttonAssignments);
  void setupChildControlsThatFit(Parameter* selectedParameter, const std::list<int>& buttonAssignments);
  void setupChildControlsForParameterWithoutButtonMapping(Parameter *selectedParameter);
  void setupChildControlsThatDontFit(Parameter *selectedParameter, const std::list<int> &buttonAssignments);

  std::optional<int> firstParameterIDOfCarousel() const;
  std::optional<int> lastParameterIDOfCarousel() const;
};


class ScaleParameterCarousel : public Carousel
{
 public:
  explicit ScaleParameterCarousel(const Rect& r);
  ~ScaleParameterCarousel() override;
  void turn() override;
  void setup(Parameter* selectedParameter) override;
  void antiTurn() override;
};