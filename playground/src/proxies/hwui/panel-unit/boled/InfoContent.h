#pragma once

#include <proxies/hwui/controls/ControlWithChildren.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/Scrollable.h>
#include <proxies/hwui/controls/Label.h>
#include <proxies/hwui/panel-unit/boled/parameter-screens/controls/MultiLineLabel.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <cassert>


class InfoContent : public Scrollable, public ControlWithChildren
{
  private:
    typedef ControlWithChildren super;
  public:
    struct MultiLineContent : public MultiLineLabel {
        MultiLineContent() : MultiLineLabel("---") {}

        virtual void setPosition (Rect rect) {
          rect.moveBy(0,2);
          MultiLineLabel::setPosition(rect);
        }

    };

    struct SingleLineContent : public LeftAlignedLabel {
        SingleLineContent(Glib::ustring name) : LeftAlignedLabel(name, Rect(0,0,0,0)) {}
        SingleLineContent() : LeftAlignedLabel("---", Rect(0,0,0,0)) {}
    };

    struct InfoField
    {
        InfoField(SingleLineContent* l, Control* c) :
            m_label(l), m_content(c)
        {
        }
        ;
        SingleLineContent* m_label;
        Control* m_content;

        void setInfo(Glib::ustring text)
        {
          if (auto label = dynamic_cast<SingleLineContent*>(m_content))
          {
            label->setText(text);
          }
          else if (auto multiLineLabel = dynamic_cast<MultiLineContent*>(m_content))
          {
            multiLineLabel->setText(text, FrameBuffer::Colors::C103);
          }
          else
          {
            assert(false);
          }
        }
        void setInfo(Glib::ustring text, FrameBuffer::Colors c)
        {
          if (auto label = dynamic_cast<SingleLineContent*>(m_content))
          {
            label->setText(text, c);
          }
          else if (auto multiLineLabel = dynamic_cast<MultiLineContent*>(m_content))
          {
            multiLineLabel->setText(text, c);
          }
          else
          {
            assert(false);
          }
        }

        void setPosition(int y) {
          auto height = std::max(m_content->getHeight(), 16);
          m_label->setPosition(Rect(0, y, 64, 16));

          if (auto label = dynamic_cast<SingleLineContent*>(m_content))
          {
            label->setPosition(Rect(64, y, 256 - 64, height));
          }
          else if (auto multiLineLabel = dynamic_cast<MultiLineContent*>(m_content))
          {
            multiLineLabel->setPosition(Rect(64, y, 256 - 64, height));
          }
          else
          {
            assert(false);
          }
        }

        int format(int y) {
          setPosition(y);
          return std::max(m_content->getPosition().getBottom(), m_label->getPosition().getBottom());
        }
    };

    InfoContent();
    virtual ~InfoContent();

    virtual void setPosition(const Rect &rect) override;
    virtual const Rect &getPosition() const override;
    virtual void setDirty() override;

    InfoField* addInfoField(std::string lineIdentifier, Glib::ustring labelText, Control* field);
    InfoField* addInfoField(std::string lineIdentifier, Glib::ustring labelText);

protected:
    std::map<std::string, std::shared_ptr<InfoField>> infoFields;
};
