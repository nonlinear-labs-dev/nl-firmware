#pragma once
#include <device-info/DeviceInformationItem.h>

class PlaycontrollerProxy;

class AftertouchCalibratedStatus : public DeviceInformationItem
{
 public:
  explicit AftertouchCalibratedStatus(DeviceInformation *parent, PlaycontrollerProxy &pcp);

  void toggle();  //for testing purposes
  bool isCalibrated() const;

  void writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const override;
  Glib::ustring get() const override;

 private:
  void onCalibrationChanged(bool calibrated);
  sigc::connection m_connection;
  bool m_isCalibrated = false;
};
