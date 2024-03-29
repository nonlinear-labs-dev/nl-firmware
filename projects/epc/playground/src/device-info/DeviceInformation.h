#pragma once

#include <http/ContentSection.h>
#include <list>
#include <memory>
#include <http/RPCActionManager.h>

class DeviceInformationItem;
class PlaycontrollerProxy;
class HardwareFeatures;

class DeviceInformation : public ContentSection
{
 public:
  explicit DeviceInformation(UpdateDocumentContributor *parent, PlaycontrollerProxy &pcp,
                             const HardwareFeatures &hwFeatures);
  virtual ~DeviceInformation();

  virtual void handleHTTPRequest(std::shared_ptr<NetworkRequest> request, const Glib::ustring &path) override;
  virtual void writeDocument(Writer &writer, tUpdateID knownRevision) const override;
  virtual Glib::ustring getPrefix() const override;

  Glib::ustring getSoftwareVersion() const;

  typedef std::shared_ptr<DeviceInformationItem> tItem;
  typedef std::list<tItem> tItemList;

  template <typename T> std::shared_ptr<T> getItem()
  {
    static_assert(std::is_base_of_v<DeviceInformationItem, T>);
    for(auto &s : m_items)
      if(std::shared_ptr<T> r = std::dynamic_pointer_cast<T>(s))
        return r;

    return nullptr;
  }

 private:
  tItemList m_items;
  RPCActionManager m_actions;
};
