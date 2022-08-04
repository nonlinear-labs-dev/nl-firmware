#include <proxies/hwui/panel-unit/boled/undo/NextUndoTransactionSiblingControl.h>
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/FrameBuffer.h>
#include <libundo/undo/Transaction.h>

NextUndoTransactionSiblingControl::NextUndoTransactionSiblingControl(const Rect &r)
    : super(r)
{
}

NextUndoTransactionSiblingControl::~NextUndoTransactionSiblingControl()
{
}

void NextUndoTransactionSiblingControl::assignTransaction(UNDO::Transaction *transaction, bool selected, bool current)
{
  super::assignTransaction(transaction, selected, current);

  if(transaction)
  {
    if(auto parent = transaction->getPredecessor())
    {
      size_t numChildren = parent->getNumSuccessors();

      if(parent->getSuccessor(numChildren - 1) != transaction)
      {
        setText(">");
        setBold(transaction->isDefaultRouteRight());
        return;
      }
    }
  }

  setText("");
}

void NextUndoTransactionSiblingControl::setBold(bool bold)
{
  if(bold != m_bold)
  {
    m_bold = bold;
    setDirty();
  }
}

std::shared_ptr<Font> NextUndoTransactionSiblingControl::getFont() const
{
  if(m_bold)
    return Fonts::get().getFont("Emphase-9-Bold", getFontHeight());

  return Fonts::get().getFont("Emphase-9-Regular", getFontHeight());
}

void NextUndoTransactionSiblingControl::setFontColor(FrameBuffer &fb) const
{
  fb.setColor(FrameBufferColors::C255);
}
