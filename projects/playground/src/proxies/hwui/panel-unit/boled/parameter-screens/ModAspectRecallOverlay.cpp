#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterModAmount.h>
#include <proxies/hwui/controls/Button.h>
#include "ModAspectRecallOverlay.h"

bool ModAspectRecallOverlay::onButton(Buttons i, bool down, ButtonModifiers mod)
{
  return false;
}

ModAspectRecallOverlay::ModAspectRecallOverlay(const Rect& r, ModulateableParameter* modP)
    : Overlay { r }
    , m_modParam { modP }
{
  constexpr static int BUTTON_VALUE_Y_POSITION = 34;
  constexpr static int BIG_SLIDER_X = 77;
  constexpr static int BIG_SLIDER_WIDTH = 102;

  addControl(new SelectedParameterModAmount(Rect(BIG_SLIDER_X + 1, 23, BIG_SLIDER_WIDTH - 2, 1)));
  addControl(new SelectedParameterModAmount(Rect(BIG_SLIDER_X + 1, 30, BIG_SLIDER_WIDTH - 2, 1)));
  addControl(new Button("<", Buttons::BUTTON_A));
  addControl(new Button("Recall", Buttons::BUTTON_B));
  addControl(new Button("Recall", Buttons::BUTTON_C));
  addControl(new Button(">", Buttons::BUTTON_D));
}

void ModAspectRecallOverlay::installMode(ModAspectRecallOverlay::Mode m)
{
  switch(m)
  {
    case Mode::MC_POS:
      break;
    case Mode::MC_SEL:
      break;
    case Mode::MC_AMT:
      break;
  }


  //  addModAmountSliders(m_modeOverlay);
  //  {
  //    if(mod->isMacroControlAssignedAndChanged())
  //    {
  //      m_modeOverlay->addControl(new RecallMCPositionLabel(Rect(0, BUTTON_VALUE_Y_POSITION, 64, 12)));
  //      m_modeOverlay->addControl(new RecallButton("Recall", Buttons::BUTTON_A));
  //    }
  //    else
  //    {
  //      m_mcPosButton->setVisible(false);
  //    }
  //    if(mod->isModSourceChanged())
  //    {
  //      m_modeOverlay->addControl(new RecallModulationSourceLabel(Rect(64, BUTTON_VALUE_Y_POSITION, 64, 12)));
  //      m_modeOverlay->addControl(new RecallButton("Recall", Buttons::BUTTON_B));
  //    }
  //    else
  //    {
  //      m_mcSelButton->setVisible(false);
  //    }
  //    if(mod->isModAmountChanged())
  //    {
  //      m_modeOverlay->addControl(new RecallMCAmountLabel(Rect(131, BUTTON_VALUE_Y_POSITION, 58, 12)));
  //      m_modeOverlay->addControl(new RecallButton("Recall", Buttons::BUTTON_C));
  //    }
  //    else
  //    {
  //      m_mcAmtButton->setVisible(false);
  //    }
  //  }
  //  else
  //  {
  //    throw std::runtime_error(to_string(__LINE__) + " has no modParam selected!");
  //  }
}
