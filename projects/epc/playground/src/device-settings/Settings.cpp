#include <Application.h>
#include <device-settings/DirectLoadSetting.h>
#include <device-settings/BaseUnitUIMode.h>
#include <device-settings/DebugLevel.h>
#include <device-settings/DeviceName.h>
#include <device-settings/NoteShift.h>
#include <device-settings/ParameterEditModeRibbonBehaviour.h>
#include <device-settings/PresetDragDropEnabled.h>
#include <device-settings/PresetStoreModeSetting.h>
#include <device-settings/SendPresetAsPlaycontrollerWriteFallback.h>
#include <device-settings/Settings.h>
#include <device-settings/VelocityCurve.h>
#include <device-settings/TransitionTime.h>
#include <device-settings/RandomizeAmount.h>
#include <device-settings/RibbonRelativeFactor.h>
#include <device-settings/EncoderAcceleration.h>
#include <device-settings/PedalType.h>
#include <device-settings/AftertouchCurve.h>
#include <device-settings/BenderCurve.h>
#include <device-settings/EditSmoothingTime.h>
#include <device-settings/SSID.h>
#include <device-settings/Passphrase.h>
#include <device-settings/PresetGlitchSuppression.h>
#include <device-settings/DateTimeAdjustment.h>
#include <device-settings/SignalFlowIndicationSetting.h>
#include <device-settings/BlockingMainThreadIndication.h>
#include <device-settings/HighlightChangedParametersSetting.h>
#include <http/NetworkRequest.h>
#include <http/UpdateDocumentMaster.h>
#include <Options.h>
#include <serialization/SettingsSerializer.h>
#include <xml/Attribute.h>
#include <xml/FileInStream.h>
#include <xml/FileOutStream.h>
#include <xml/Writer.h>
#include <xml/XmlReader.h>
#include <xml/XmlWriter.h>
#include <xml/VersionAttribute.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include "EpcWifi.h"
#include "WifiSetting.h"
#include "SettingsActions.h"
#include "CrashOnError.h"
#include "TuneReference.h"
#include "TotalRAM.h"
#include "UsedRAM.h"
#include "SyncVoiceGroupsAcrossUIS.h"
#include "SplitPointSyncParameters.h"
#include "GlobalLocalEnableSetting.h"
#include "FocusAndModeSetting.h"
#include "SelectedRibbonsSetting.h"
#include "AftertouchLegacyMode.h"
#include "BaseUnitUIDetail.h"
#include "SendActiveSensingSetting.h"
#include <presets/PresetManager.h>
#include <presets/EditBuffer.h>
#include <parameter_declarations.h>
#include <parameters/RibbonParameter.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>
#include <iostream>
#include <device-settings/midi/MidiChannelSettings.h>
#include <device-settings/midi/receive/MidiReceiveAftertouchCurveSetting.h>
#include <device-settings/midi/receive/MidiReceiveVelocityCurveSetting.h>
#include <device-settings/midi/mappings/AftertouchCCMapping.h>
#include <device-settings/midi/mappings/BenderCCMapping.h>
#include <device-settings/midi/mappings/PedalCCMapping.h>
#include <device-settings/midi/mappings/RibbonCCMapping.h>
#include <device-settings/midi/mappings/EnableHighVelocityCC.h>
#include <device-settings/midi/mappings/Enable14BitSupport.h>
#include <device-settings/flac/AutoStartRecorderSetting.h>
#include <device-settings/flac/FlacRecorderVirgin.h>
#include <device-settings/midi/RoutingSettings.h>
#include <device-settings/AlsaFramesPerPeriod.h>
#include <proxies/hwui/HardwareFeatures.h>

Settings::Settings(const Glib::ustring &file, UpdateDocumentMaster *master, const HardwareFeatures &hwFeatures)
    : UpdateDocumentContributor(master)
    , m_file(file)
    , m_saveJob(5000, [this] { save(); })
{
  addSetting("DirectLoad", new DirectLoadSetting(*this));
  addSetting("SendPresetAsLPCWriteFallback", new SendPresetAsPlaycontrollerWriteFallback(*this));
  addSetting("PresetStoreModeSetting", new PresetStoreModeSetting(*this));
  addSetting("BaseUnitUIMode", new BaseUnitUIMode(*this));
  addSetting("BaseUnitUIDetail", new BaseUnitUIDetail(*this));
  addSetting("PanelUnitFocusAndMode", new FocusAndModeSetting(*this));
  addSetting("NoteShift", new NoteShift(*this));
  addSetting("ParameterEditModeRibbonBehaviour", new ParameterEditModeRibbonBehaviour(*this));
  addSetting("DebugLevel", new DebugLevel(*this));
  addSetting("VelocityCurve", new VelocityCurve(*this));
  addSetting("DeviceName", new DeviceName(*this));
  addSetting("PresetDragDropEnabled", new PresetDragDropEnabled(*this));
  addSetting("TransitionTime", new TransitionTime(*this));
  addSetting("RandomizeAmount", new RandomizeAmount(*this));
  addSetting("RibbonRelFactor", new RibbonRelativeFactor(*this));
  addSetting("Pedal1Type", new PedalType(*this, 1));
  addSetting("Pedal2Type", new PedalType(*this, 2));
  addSetting("Pedal3Type", new PedalType(*this, 3));
  addSetting("Pedal4Type", new PedalType(*this, 4));
  addSetting("EncoderAcceleration", new EncoderAcceleration(*this));
  addSetting("AftertouchCurve", new AftertouchCurve(*this));
  addSetting("BenderCurve", new BenderCurve(*this));
  addSetting("EditSmoothingTime", new EditSmoothingTime(*this));

  addSetting("Passphrase", new Passphrase(*this));
  addSetting("WifiSetting", new WifiSetting(*this));

  addSetting("PresetGlitchSuppression", new PresetGlitchSuppression(*this));
  addSetting("DateTimeAdjustment", new DateTimeAdjustment(*this));
  addSetting("SignalFlowIndication", new SignalFlowIndicationSetting(*this));
  addSetting("IndicateBlockedUI", new BlockingMainThreadIndication(*this, false));
  addSetting("HighlightChangedParameters", new HighlightChangedParametersSetting(*this));
  addSetting("ForceHighlightChangedParameters", new ForceHighlightChangedParametersSetting(*this));
  addSetting("CrashOnError", new CrashOnError(*this));
  addSetting("TuneReference", new TuneReference(*this));

  addSetting("SyncVoiceGroups", new SyncVoiceGroupsAcrossUIS(*this));
  addSetting("ScreenSaverTimeout", new ScreenSaverTimeoutSetting(*this));
  addSetting("SyncSplit", new SplitPointSyncParameters(*this));

  addSetting("ReceiveChannel", new MidiReceiveChannelSetting(*this));
  addSetting("ReceiveChannelSplit", new MidiReceiveChannelSplitSetting(*this));
  addSetting("SendChannel", new MidiSendChannelSetting(*this));
  addSetting("SendChannelSplit", new MidiSendChannelSplitSetting(*this));

  //Unused:
  addSetting("ReceiveAftertouchCurve", new MidiReceiveAftertouchCurveSetting(*this));
  addSetting("ReceiveVelocityCurve", new MidiReceiveVelocityCurveSetting(*this));

  addSetting("HighResCC", new Enable14BitSupport(*this));
  auto enable14Bit = getSetting<Enable14BitSupport>();

  addSetting("Pedal1Mapping", new PedalCCMapping<1>(*this, *enable14Bit));
  addSetting("Pedal2Mapping", new PedalCCMapping<2>(*this, *enable14Bit));
  addSetting("Pedal3Mapping", new PedalCCMapping<3>(*this, *enable14Bit));
  addSetting("Pedal4Mapping", new PedalCCMapping<4>(*this, *enable14Bit));
  addSetting("Ribbon1Mapping", new RibbonCCMapping<1>(*this, *enable14Bit));
  addSetting("Ribbon2Mapping", new RibbonCCMapping<2>(*this, *enable14Bit));
  addSetting("Ribbon3Mapping", new RibbonCCMapping<3>(*this, *enable14Bit));
  addSetting("Ribbon4Mapping", new RibbonCCMapping<4>(*this, *enable14Bit));
  addSetting("BenderMapping", new BenderCCMapping(*this, *enable14Bit));
  addSetting("AftertouchMapping", new AftertouchCCMapping(*this, *enable14Bit));

  addSetting("HighVeloCC", new EnableHighVelocityCC(*this));
  addSetting("AutoStartRecorder", new AutoStartRecorderSetting(*this));
  addSetting("RoutingSettings", new RoutingSettings(*this));
  addSetting("GlobalLocalEnable", new GlobalLocalEnableSetting(*this));

  addSetting("AlsaFramesPerPeriod", new AlsaFramesPerPeriod(*this));

  addSetting("FlacRecorderVirgin", new FlacRecorderVirgin(*this));
  addSetting("SelectedRibbons", new SelectedRibbonsSetting(*this));
  addSetting("AftertouchLegacyMode", new AftertouchLegacyMode(*this));

  addSetting("SendActiveSensing", new SendActiveSensingSetting(*this));
}

Settings::~Settings()
{
  save();
}

Settings::tUpdateID Settings::onChange(uint64_t flags)
{
  m_saveJob.trigger();
  m_sigChanged.emit();
  return UpdateDocumentContributor::onChange(flags);
}

void Settings::init()
{
  load();

  for(auto &s : getSettings())
  {
    s.second->init();
  }

  getSetting<FocusAndModeSetting>()->setFocusAndModeFreeze(true);
}

void Settings::reload()
{
  load();
}

void Settings::load()
{
  auto lock = m_isLoading.lock();

  nltools::Log::info(__PRETTY_FUNCTION__, G_STRLOC);

  try
  {
    nltools::Log::info(__PRETTY_FUNCTION__, G_STRLOC);
    FileInStream in(m_file, false);
    XmlReader reader(in, nullptr);
    reader.read<SettingsSerializer>(std::ref(*this));
  }
  catch(...)
  {
    nltools::Log::error("Exception loading the settings!");
  }

  nltools::Log::info(__PRETTY_FUNCTION__, G_STRLOC);

  sanitize();

  using nltools::msg::EndPoint;
  if(m_aeSettingsConnection.connected())
  {
    m_aeSettingsConnection.disconnect();
  }

  m_aeSettingsConnection = onConnectionEstablished(EndPoint::AudioEngine, [&] { sendGlobalAESettings(); });
}

void Settings::save()
{
  try
  {
    SettingsSerializer serializer(*this);
    FileOutStream stream(m_file, false);
    XmlWriter writer(stream);
    serializer.write(writer, VersionAttribute::get());
  }
  catch(...)
  {
    nltools::Log::error("Could not save Settings to", m_file);
  }
}

void Settings::sanitize()
{
  getSetting<DeviceName>()->sanitize(getSetting<DateTimeAdjustment>());
}

void Settings::addSetting(const Glib::ustring &key, Setting *s)
{
  m_settings[key] = tSettingPtr(s);
}

const Settings::tMap &Settings::getSettings() const
{
  return m_settings;
}

Setting *Settings::getSetting(const Glib::ustring &key)
{
  auto it = m_settings.find(key);

  if(it != m_settings.end())
    return it->second.get();

  return nullptr;
}

void Settings::writeDocument(Writer &writer, tUpdateID knownRevision) const
{
  bool changed = knownRevision < getUpdateIDOfLastChange();

  writer.writeTag("settings", Attribute("changed", changed),
                  [&]()
                  {
                    if(changed)
                    {
                      for(auto &setting : m_settings)
                      {
                        writer.writeTag(setting.first, [&]() { setting.second->writeDocument(writer, knownRevision); });
                      }
                    }
                  });
}

bool Settings::isLoading() const
{
  return m_isLoading.isLocked();
}

void Settings::sendSettingsToPlaycontroller(SendReason reason)
{
  if(reason == SendReason::HeartBeatDropped)
  {
    sendGlobalPlaycontrollerInitSettings();
    return;
  }
}

void Settings::sendGlobalPlaycontrollerInitSettings()
{
  getSetting<BaseUnitUIMode>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<RibbonRelativeFactor>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<ParameterEditModeRibbonBehaviour>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<VelocityCurve>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<AftertouchCurve>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<BenderCurve>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting("Pedal1Type")->syncExternals(SendReason::HeartBeatDropped);
  getSetting("Pedal2Type")->syncExternals(SendReason::HeartBeatDropped);
  getSetting("Pedal3Type")->syncExternals(SendReason::HeartBeatDropped);
  getSetting("Pedal4Type")->syncExternals(SendReason::HeartBeatDropped);
  getSetting<AftertouchLegacyMode>()->syncExternals(SendReason::HeartBeatDropped);
}

void Settings::sendGlobalAESettings()
{
  getSetting<PresetGlitchSuppression>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<TransitionTime>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<NoteShift>()->syncExternals(SendReason::HeartBeatDropped);
  getSetting<TuneReference>()->syncExternals(SendReason::HeartBeatDropped);
}

void Settings::sendPresetSettingsToPlaycontroller()
{
  auto eb = Application::get().getPresetManager()->getEditBuffer();
  auto r1 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_1, VoiceGroup::Global }));
  auto r2 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_2, VoiceGroup::Global }));
  auto r3 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_3, VoiceGroup::Global }));
  auto r4 = dynamic_cast<RibbonParameter *>(eb->findParameterByID({ C15::PID::Ribbon_4, VoiceGroup::Global }));
  r1->sendModeToPlaycontroller();
  r2->sendModeToPlaycontroller();
  r3->sendModeToPlaycontroller();
  r4->sendModeToPlaycontroller();
}

sigc::connection Settings::onSettingsChanged(sigc::slot<void(void)> s)
{
  return m_sigChanged.connect(s);
}
