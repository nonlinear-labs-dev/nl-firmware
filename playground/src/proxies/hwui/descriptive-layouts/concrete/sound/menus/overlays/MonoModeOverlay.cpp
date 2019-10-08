#include <Application.h>
#include <presets/EditBuffer.h>
#include <presets/PresetManager.h>
#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/ParameterItem.h>
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/controls/OverlayParameterLabel.h>
#include "MonoModeOverlay.h"
#include <proxies/hwui/descriptive-layouts/concrete/sound/menus/items/MonoParameterItem.h>

void sanitizeIndex(size_t& index, size_t maxSize)
{
  index = std::max<size_t>(std::min<size_t>(index, maxSize - 1), 0);
}

MonoModeOverlay::MonoModeOverlay(const Rect& r)
    : MenuOverlay(r)
{
  const auto ids = std::vector<Parameter::ID>{ 12345, 12346, 12347, 12348 };

  int i = 0;
  for(auto& id : ids)
  {
    const auto height = r.getHeight() / ids.size();
    const auto baseRect
        = Rect{ r.getLeft(), static_cast<int>(r.getTop() + height * i), r.getWidth(), static_cast<int>(height) };
    auto param = getParameter(id);
    m_labels.emplace_back(addControl(new MonoParameterItem(param->getLongName(), param, baseRect)));
    i++;
  }

  sanitizeIndex(selection, ids.size());
  m_labels[selection]->setHighlight(true);
}

bool MonoModeOverlay::onButton(Buttons i, bool down, ButtonModifiers mod)
{
  if(down)
  {
    switch(i)
    {
      case Buttons::BUTTON_A:
      case Buttons::BUTTON_B:
        return exitCurr();
      case Buttons::BUTTON_C:
      case Buttons::BUTTON_D:
        enterCurr();
        return true;
      case Buttons::BUTTON_DEC:
        selectPrev();
        return true;
      case Buttons::BUTTON_INC:
        selectNext();
        return true;
    }
  }
  return false;
}

Parameter* MonoModeOverlay::getParameter(Parameter::ID id) const
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  return eb->findParameterByID(id);
}

void MonoModeOverlay::selectNext()
{
  m_labels[selection]->setHighlight(false);
  sanitizeIndex(++selection, m_labels.size());
  m_labels[selection]->setHighlight(true);
}

void MonoModeOverlay::selectPrev()
{
  m_labels[selection]->setHighlight(false);
  sanitizeIndex(--selection, m_labels.size());
  m_labels[selection]->setHighlight(true);
}

bool MonoModeOverlay::redraw(FrameBuffer& fb)
{
  MenuOverlay::redraw(fb);

  if(m_overlay)
  {
    return m_overlay->redraw(fb);
  }
  else
  {
    auto ret = MenuOverlay::redraw(fb);
    for(auto& i : m_labels)
    {
      i->redraw(fb);
    }
    return ret;
  }
}

void MonoModeOverlay::enterCurr()
{
  if(!m_overlay)
  {
    if(auto& p = m_labels[selection])
    {
      m_overlay = dynamic_cast<MonoParameterOverlay*>(addControl(p->createOverlay()));
    }
  }
}

bool MonoModeOverlay::exitCurr()
{
  if(m_overlay)
  {
    remove(m_overlay);
    m_overlay = nullptr;
    setDirty();
    return true;
  }
  return false;
}
