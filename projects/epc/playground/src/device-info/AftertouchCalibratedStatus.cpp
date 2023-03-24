#include "AftertouchCalibratedStatus.h"
#include <proxies/playcontroller/PlaycontrollerProxy.h>

AftertouchCalibratedStatus::AftertouchCalibratedStatus(DeviceInformation *parent, PlaycontrollerProxy &pcp)
    : DeviceInformationItem(parent)
{
  m_connection = pcp.onCalibrationStatusChanged(sigc::mem_fun(this, &AftertouchCalibratedStatus::onCalibrationChanged));
}

void AftertouchCalibratedStatus::onCalibrationChanged(bool calibrated)
{
  m_isCalibrated = calibrated;
  onChange();
}

void AftertouchCalibratedStatus::writeDocument(Writer &writer, UpdateDocumentContributor::tUpdateID knownRevision) const
{
  writer.writeTextElement("at-calibrated", get());
}

Glib::ustring AftertouchCalibratedStatus::get() const
{
  return m_isCalibrated ? "on" : "off";
}

void AftertouchCalibratedStatus::toggle()
{
  onCalibrationChanged(!m_isCalibrated);
}

bool AftertouchCalibratedStatus::isCalibrated() const
{
  return m_isCalibrated;
}
