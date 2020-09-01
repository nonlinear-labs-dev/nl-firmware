#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/SelectedParameterModAmount.h>
#include <proxies/hwui/controls/Button.h>
#include <parameters/MacroControlParameter.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulateableParameterRecallControls/RecallMCPositionLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MCPositionLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulateableParameterRecallControls/RecallMCAmountLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MCAmountLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulationSourceLabel.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/ModulateableParameterRecallControls/RecallModulationSourceLabel.h>
#include "ModAspectRecallOverlay.h"
#include <proxies/hwui/FrameBuffer.h>

bool ModAspectRecallOverlay::onButton(Buttons i, bool down, ButtonModifiers mod)
{
  if(!down)
    return false;

  if(i == Buttons::BUTTON_D)
  {
    nextMode();
    return true;
  }
  if(i == Buttons::BUTTON_A)
  {
    previousMode();
    return true;
  }

  if(m_buttonB->getText().text == "Recall" && i == Buttons::BUTTON_B)
  {
    doRecall();
    return true;
  }

  if(m_buttonC->getText().text == "Recall" && i == Buttons::BUTTON_C)
  {
    undoRecall();
    return true;
  }

  return false;
}

ModAspectRecallOverlay::ModAspectRecallOverlay(const Rect& r, ModulateableParameter* modP, Mode openMode)
    : Overlay { r }
    , m_modParam { modP }
{
  constexpr static int BUTTON_VALUE_Y_POSITION = 34;
  constexpr static int BIG_SLIDER_X = 77;
  constexpr static int BIG_SLIDER_WIDTH = 102;

  addControl(new SelectedParameterModAmount(Rect(BIG_SLIDER_X + 1, 23, BIG_SLIDER_WIDTH - 2, 1)));
  addControl(new SelectedParameterModAmount(Rect(BIG_SLIDER_X + 1, 30, BIG_SLIDER_WIDTH - 2, 1)));

  m_labelA = addControl(new Label({ 0, 35, 64, 12 }));

  m_buttonA = addControl(new Button("<", Buttons::BUTTON_A));
  m_buttonB = addControl(new Button("Recall", Buttons::BUTTON_B));
  m_buttonC = addControl(new Button("Recall", Buttons::BUTTON_C));
  m_buttonD = addControl(new Button(">", Buttons::BUTTON_D));

  m_oldRecallValues[Mode::MC_POS] = { false, 0.0 };
  m_oldRecallValues[Mode::MC_SEL] = { false, MacroControls::NONE };
  m_oldRecallValues[Mode::MC_AMT] = { false, 0.0 };

  installMode(openMode);
}

int ModAspectRecallOverlay::getZPriority() const
{
  return 1;
}

bool ModAspectRecallOverlay::redraw(FrameBuffer& fb)
{
  drawBackground(fb);
  return ControlWithChildren::redraw(fb);
}

void ModAspectRecallOverlay::drawBackground(FrameBuffer& fb)
{
  fb.setColor(FrameBufferColors::C43);
  auto pos = getPosition();
  pos.setLeft(pos.getRight() - 64);
  fb.fillRect(pos);
}

void ModAspectRecallOverlay::installMode(ModAspectRecallOverlay::Mode m)
{
  m_mode = m;

  remove(m_labelB);
  remove(m_labelC);

  auto& recall = m_oldRecallValues[m];

  const auto mcPosChanged = m_modParam->getMacroControl()->isValueChangedFromLoaded();
  const auto modAmtChanged = m_modParam->isModAmountChanged();
  const auto modSelChanged = m_modParam->isModSourceChanged();

  switch(m)
  {
    case Mode::MC_POS:
      m_labelA->setText("MC Position");
      m_labelB = addControl(new RecallMCPositionLabel(Rect(64, 34, 64, 12)));
      m_labelC = addControl(new MCPositionLabel({ 128, 34, 64, 12 }));

      if(mcPosChanged)
      {
        m_buttonB->setText("Recall");
        m_buttonC->setText("");

        m_labelB->setHighlight(false);
        m_labelC->setHighlight(true);
      }
      else if(recall.wasRecalled)
      {
        m_labelC->setText({ std::to_string(std::get<MacroControls>(recall.old)), 0 });
        m_buttonB->setText("");
        m_buttonC->setText("Recall");

        m_labelB->setHighlight(true);
        m_labelC->setHighlight(false);
      }

      if(modAmtChanged || modSelChanged)
      {
        m_buttonA->setText("<");
        m_buttonD->setText(">");
      }
      else
      {
        m_buttonA->setText("");
        m_buttonD->setText("");
      }

      break;
    case Mode::MC_SEL:
      m_labelA->setText("MC Selection");
      m_labelB = addControl(new RecallModulationSourceLabel(Rect(64, 34, 64, 12)));
      m_labelC = addControl(new ModulationSourceLabel({ 128, 34, 64, 12 }));

      if(modSelChanged)
      {
        m_buttonB->setText("Recall");
        m_buttonC->setText("");

        m_labelB->setHighlight(false);
        m_labelC->setHighlight(true);
      }
      else if(recall.wasRecalled)
      {
        m_labelC->setText({ std::to_string(std::get<float>(recall.old)), 0 });

        m_buttonB->setText("");
        m_buttonC->setText("Recall");

        m_labelB->setHighlight(false);
        m_labelC->setHighlight(true);
      }

      if(modAmtChanged || mcPosChanged)
      {
        m_buttonA->setText("<");
        m_buttonD->setText(">");
      }
      else
      {
        m_buttonA->setText("");
        m_buttonD->setText("");
      }

      break;
    case Mode::MC_AMT:
      m_labelA->setText("MC Amount");
      m_labelB = addControl(new RecallMCAmountLabel(Rect(64, 34, 64, 12)));
      m_labelC = addControl(new MCAmountLabel({ 128, 34, 64, 12 }));

      if(modAmtChanged)
      {
        m_buttonB->setText("Recall");
        m_buttonC->setText("");

        m_labelB->setHighlight(false);
        m_labelC->setHighlight(true);
      }
      else if(recall.wasRecalled)
      {
        m_labelC->setText({ std::to_string(std::get<float>(recall.old)), 0 });

        m_buttonB->setText("");
        m_buttonC->setText("Recall");

        m_labelB->setHighlight(true);
        m_labelC->setHighlight(false);
      }

      if(modSelChanged || mcPosChanged)
      {
        m_buttonA->setText("<");
        m_buttonD->setText(">");
      }
      else
      {
        m_buttonA->setText("");
        m_buttonD->setText("");
      }
      break;
  }
}

void ModAspectRecallOverlay::doRecall()
{
  auto& recall = m_oldRecallValues[m_mode];

  auto oldValB = m_labelB->getText();
  auto oldValC = m_labelC->getText();

  recall.wasRecalled = true;

  switch(m_mode)
  {
    case Mode::MC_SEL:
      recall.old = m_modParam->getModulationSource();
      m_modParam->undoableRecallMCSource();
      break;
    case Mode::MC_AMT:
      recall.old = m_modParam->getModulationAmount();
      m_modParam->undoableRecallMCAmount();
      break;
    case Mode::MC_POS:
      recall.old = m_modParam->getMacroControl()->getControlPositionValue();
      m_modParam->undoableRecallMCPos();
      break;
  }

  m_labelB->setText(oldValC);
  m_labelC->setText(oldValB);

  m_labelB->setHighlight(false);
  m_labelC->setHighlight(true);

  m_buttonB->setText("");
  m_buttonC->setText("Recall");
}

void ModAspectRecallOverlay::undoRecall()
{
  auto& recall = m_oldRecallValues[m_mode];
  recall.wasRecalled = false;

  auto oldValB = m_labelB->getText();
  auto oldValC = m_labelC->getText();

  switch(m_mode)
  {
    case Mode::MC_SEL:
      m_modParam->undoableUndoRecallMCSel(std::get<MacroControls>(recall.old));
      break;
    case Mode::MC_AMT:
      m_modParam->undoableUndoRecallMCAmount(std::get<float>(recall.old));
      break;
    case Mode::MC_POS:
      m_modParam->undoableUndoRecallMCPos(std::get<float>(recall.old));
      break;
  }

  m_labelB->setText(oldValC);
  m_labelC->setText(oldValB);

  m_labelB->setHighlight(false);
  m_labelC->setHighlight(true);

  m_buttonB->setText("Recall");
  m_buttonC->setText("");
}

void ModAspectRecallOverlay::previousMode()
{
  switch(m_mode)
  {
    case MC_POS:
      if(m_modParam->isModAmountChanged())
        installMode(MC_AMT);
      else if(m_modParam->isModSourceChanged())
        installMode(MC_SEL);
      break;
    case MC_SEL:
      if(m_modParam->getMacroControl()->isValueChangedFromLoaded())
        installMode(MC_POS);
      else if(m_modParam->isModAmountChanged())
        installMode(MC_AMT);
      break;
    case MC_AMT:
      if(m_modParam->isModSourceChanged())
        installMode(MC_SEL);
      else if(m_modParam->getMacroControl()->isValueChangedFromLoaded())
        installMode(MC_POS);
      break;
  }
}

void ModAspectRecallOverlay::nextMode()
{
  switch(m_mode)
  {
    case MC_POS:
      if(m_modParam->isModSourceChanged())
        installMode(MC_SEL);
      else if(m_modParam->isModAmountChanged())
        installMode(MC_AMT);
      break;
    case MC_SEL:
      if(m_modParam->isModAmountChanged())
        installMode(MC_AMT);
      else if(m_modParam->getMacroControl()->isValueChangedFromLoaded())
        installMode(MC_POS);
      break;
    case MC_AMT:
      if(m_modParam->getMacroControl()->isValueChangedFromLoaded())
        installMode(MC_POS);
      else if(m_modParam->isModSourceChanged())
        installMode(MC_SEL);
      break;
  }
}
