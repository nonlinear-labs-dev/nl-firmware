#include <Application.h>
#include <device-info/DateTimeInfo.h>
#include <device-info/SoftwareVersion.h>
#include <device-info/BufferUnderruns.h>
#include <device-settings/PedalType.h>
#include <groups/HardwareSourcesGroup.h>
#include <parameters/PedalParameter.h>
#include <presets/EditBuffer.h>
#include <proxies/hwui/FrameBuffer.h>
#include <proxies/hwui/HWUI.h>
#include <proxies/hwui/Oleds.h>
#include <proxies/hwui/panel-unit/boled/BOLED.h>
#include <proxies/hwui/panel-unit/boled/setup/AboutLayout.h>
#include <proxies/hwui/panel-unit/boled/setup/AftertouchEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/AftertouchView.h>
#include <proxies/hwui/panel-unit/boled/setup/BenderCurveEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/BenderCurveView.h>
#include <proxies/hwui/panel-unit/boled/setup/DeviceInfoItemView.h>
#include <proxies/hwui/panel-unit/boled/setup/DeviceNameView.h>
#include <proxies/hwui/panel-unit/boled/setup/DateTimeEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/EditSmoothingTimeEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/EditSmoothingTimeView.h>
#include <proxies/hwui/panel-unit/boled/setup/EncoderAccelerationEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/EncoderAccelerationView.h>
#include <proxies/hwui/panel-unit/boled/setup/ExportBackupEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/FreeInternalMemoryView.h>
#include <proxies/hwui/panel-unit/boled/setup/PassphraseEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/PassphraseView.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalSelectionControl.h>
#include <proxies/hwui/panel-unit/boled/setup/PedalView.h>
#include <proxies/hwui/panel-unit/boled/setup/RenameDeviceLayout.h>
#include <proxies/hwui/panel-unit/boled/setup/RibbonRelativeFactorSettingEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/RibbonRelativeFactorSettingView.h>
#include <proxies/hwui/panel-unit/boled/setup/SetupEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/SetupLayout.h>
#include <proxies/hwui/panel-unit/boled/setup/SetupSelectionLabel.h>
#include <proxies/hwui/panel-unit/boled/setup/SSIDView.h>
#include <proxies/hwui/panel-unit/boled/setup/UpdateAvailableView.h>
#include <proxies/hwui/panel-unit/boled/setup/UpdateAvailableEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/VelocityEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/VelocityView.h>
#include <proxies/hwui/panel-unit/boled/setup/USBStickAvailableView.h>
#include <proxies/hwui/panel-unit/boled/setup/ExportBackupView.h>
#include <proxies/hwui/panel-unit/boled/setup/ImportBackupView.h>
#include <proxies/hwui/panel-unit/boled/setup/ImportBackupEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/WiFiSettingEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/SettingView.h>
#include <proxies/hwui/panel-unit/boled/setup/SettingEditors.h>
#include <proxies/hwui/panel-unit/boled/setup/NumericSettingEditor.h>
#include <proxies/hwui/panel-unit/boled/setup/WiFiSettingView.h>
#include <proxies/hwui/panel-unit/EditPanel.h>
#include <proxies/hwui/panel-unit/PanelUnit.h>
#include <proxies/hwui/TextEditUsageMode.h>
#include <proxies/hwui/controls/LeftAlignedLabel.h>
#include <proxies/playcontroller/PlaycontrollerProxy.h>
#include <xml/FileOutStream.h>
#include <bitset>
#include <chrono>
#include <list>
#include <memory>
#include <utility>
#include <device-settings/TuneReference.h>
#include <device-settings/TransitionTime.h>
#include <tools/StringTools.h>
#include <device-settings/SyncVoiceGroupsAcrossUIS.h>
#include "UISoftwareVersionEditor.h"
#include "ScreenSaverTimeControls.h"
#include "RoutingsView.h"
#include "RoutingsEditor.h"
#include "OneShotEntryTypes.h"
#include "use-cases/SoundUseCases.h"
#include "use-cases/PresetManagerUseCases.h"

#include <proxies/hwui/descriptive-layouts/concrete/menu/menu-items/AnimatedGenericItem.h>
#include <device-settings/midi/MidiChannelSettings.h>
#include <device-settings/midi/mappings/PedalCCMapping.h>
#include <device-settings/midi/mappings/RibbonCCMapping.h>
#include <device-settings/midi/mappings/BenderCCMapping.h>
#include <device-settings/midi/mappings/AftertouchCCMapping.h>
#include <device-settings/midi/mappings/EnableHighVelocityCC.h>
#include <device-settings/midi/mappings/Enable14BitSupport.h>
#include <device-settings/flac/AutoStartRecorderSetting.h>
#include <device-settings/PresetGlitchSuppression.h>

#include <presets/Bank.h>
#include <use-cases/SettingsUseCases.h>
#include <device-settings/ScreenSaverTimeoutSetting.h>
#include <device-settings/UsedRAM.h>
#include <device-settings/TotalRAM.h>
#include <device-settings/midi/RoutingSettings.h>
#include <device-settings/SignalFlowIndicationSetting.h>
#include <device-settings/GlobalLocalEnableSetting.h>
#include <device-info/UniqueHardwareID.h>

namespace NavTree
{
  struct InnerNode;

  struct Node
  {
    Node(InnerNode *parent, Glib::ustring name)
        : parent(parent)
        , name(std::move(name))
    {
    }

    virtual ~Node() = default;

    virtual Control *createSelectionControl()
    {
      return new SetupSelectionLabel(name);
    }

    virtual bool onEditModeEntered()
    {
      return false;
    }

    virtual Node *getDesiredFocusChangeOnEditModeExited()
    {
      return nullptr;
    }

    [[nodiscard]] virtual Glib::ustring getName() const
    {
      return name;
    }

    InnerNode *parent;
    virtual Control *createView() = 0;

   protected:
    Glib::ustring name;
  };

  struct Leaf : Node
  {
    Leaf(InnerNode *parent, Glib::ustring name)
        : Node(parent, std::move(name))
    {
    }
  };

  struct EditableLeaf : Leaf
  {
    EditableLeaf(InnerNode *parent, Glib::ustring name)
        : Leaf(parent, std::move(name))
    {
    }

    virtual Control *createEditor() = 0;
  };

  struct InnerNode : Node
  {
    InnerNode(InnerNode *parent, Glib::ustring name)
        : Node(parent, std::move(name))
    {
    }

    Control *createView() override
    {
      return new SetupLabel("...", Rect(0, 0, 0, 0));
    }

    std::list<std::unique_ptr<Node>> children;
  };

  template <typename tSetting> struct EnumSettingItem : EditableLeaf
  {
    EnumSettingItem(InnerNode *parent, const std::string &text)
        : EditableLeaf(parent, text)
    {
    }

    Control *createView() override
    {
      return new SettingView<tSetting>();
    }

    Control *createEditor() override
    {
      constexpr auto isBoolSetting = std::is_base_of_v<BooleanSetting, tSetting>;
      constexpr auto isOnOffSetting = std::is_base_of_v<PresetGlitchSuppression, tSetting>;

      if constexpr(isBoolSetting || isOnOffSetting)
      {
        return new OnOffOrderChangedEnumSettingEditor<tSetting>();
      }
      else
      {
        return new EnumSettingEditor<tSetting>();
      }
    }
  };

  struct Velocity : EditableLeaf
  {
    explicit Velocity(InnerNode *parent)
        : EditableLeaf(parent, "Velocity Curve")
    {
    }

    Control *createView() override
    {
      return new VelocityView();
    }

    Control *createEditor() override
    {
      return new VelocityEditor();
    }
  };

  struct Aftertouch : EditableLeaf
  {
    explicit Aftertouch(InnerNode *parent)
        : EditableLeaf(parent, "Aftertouch Curve")
    {
    }

    Control *createView() override
    {
      return new AftertouchView();
    }

    Control *createEditor() override
    {
      return new AftertouchEditor();
    }
  };

  struct BenderCurveSetting : EditableLeaf
  {
    explicit BenderCurveSetting(InnerNode *parent)
        : EditableLeaf(parent, "Bender Curve")
    {
    }

    Control *createView() override
    {
      return new BenderCurveView();
    }

    Control *createEditor() override
    {
      return new BenderCurveEditor();
    }
  };

  struct PedalSetting : EditableLeaf
  {
    PedalSetting(InnerNode *parent, int id)
        : EditableLeaf(parent, "")
    {
      param = dynamic_cast<PedalParameter *>(
          Application::get().getPresetManager()->getEditBuffer()->findParameterByID({ id, VoiceGroup::Global }));
      name = param->getLongName();
    }

    Control *createView() override
    {
      return new PedalView(param->getAssociatedPedalTypeSetting());
    }

    Control *createEditor() override
    {
      return new PedalEditor(param->getAssociatedPedalTypeSetting());
    }

    Control *createSelectionControl() override
    {
      return new PedalSelectionControl(param);
    }

    PedalParameter *param;
  };

  struct WiFiSetting : EditableLeaf
  {
    explicit WiFiSetting(InnerNode *parent)
        : EditableLeaf(parent, "Enable/Disable WiFi")
    {
    }

    Control *createView() override
    {
      return new WiFiSettingView();
    }

    Control *createEditor() override
    {
      return new WiFiSettingEditor();
    }
  };

  struct OneShotEntry : EditableLeaf
  {
    struct Item : public AnimatedGenericItem
    {
      Item(const Rect &rect, OneShotTypes::StartCB startCB, OneShotTypes::FinishCB finishCB)
          : AnimatedGenericItem("", rect, startCB, finishCB)
      {
      }

      bool drawHighlightBorder(FrameBuffer &) override
      {
        return false;
      }
    };

    Item *theItem = nullptr;

    OneShotTypes::StartCB m_startCB;
    OneShotTypes::FinishCB m_finishedCB;

    OneShotEntry(InnerNode *p, const std::string &name, OneShotTypes::StartCB cb)
        : EditableLeaf(p, name)
        , m_startCB(std::move(cb))
    {
    }

    OneShotEntry(InnerNode *p, const std::string &name, OneShotTypes::FinishCB cb)
        : EditableLeaf(p, name)
        , m_finishedCB(std::move(cb))
    {
    }

    OneShotEntry(InnerNode *p, const std::string &name, OneShotTypes::StartCB sCB, OneShotTypes::FinishCB fCB)
        : EditableLeaf(p, name)
        , m_startCB(std::move(sCB))
        , m_finishedCB(std::move(fCB))
    {
    }

    Control *createView() override
    {
      theItem = new Item(Rect(0, 0, 0, 0), m_startCB, m_finishedCB);
      return theItem;
    }

    Control *createEditor() override
    {
      return nullptr;
    }

    bool onEditModeEntered() override
    {
      if(theItem)
        theItem->doAction();
      return true;
    }
  };

  struct StoreInitSound : OneShotEntry
  {
    explicit StoreInitSound(InnerNode *p)
        : OneShotEntry(p, "Store Init Sound", OneShotTypes::StartCB([] {
          auto pm = Application::get().getPresetManager();
          SoundUseCases useCases(pm->getEditBuffer(), pm);
          useCases.storeInitSound();
        }))
    {
    }
  };

  struct ResetInitSound : OneShotEntry
  {
    explicit ResetInitSound(InnerNode *p)
        : OneShotEntry(p, "Reset Init Sound", OneShotTypes::StartCB([] {
          auto pm = Application::get().getPresetManager();
          SoundUseCases useCases(pm->getEditBuffer(), pm);
          useCases.resetInitSound();
        }))
    {
    }
  };

  template <typename tSetting> struct SettingItem : EditableLeaf
  {
   private:
    tSetting *getSetting()
    {
      return Application::get().getSettings()->getSetting<tSetting>().get();
    }

   public:
    SettingItem(InnerNode *parent, const char *name)
        : EditableLeaf(parent, name)
    {
    }

    Control *createView() override
    {
      return new SettingView<tSetting>();
    }

    Control *createEditor() override
    {
      if constexpr(std::is_base_of_v<BooleanSetting, tSetting>)
      {
        return new BooleanSettingEditor<tSetting>();
      }
      else
      {
        return new NumericSettingEditor<tSetting>();
      }
    }
  };

  struct EditSmoothingTime : EditableLeaf
  {
    explicit EditSmoothingTime(InnerNode *parent)
        : EditableLeaf(parent, "Edit Smoothing Time")
    {
    }

    Control *createView() override
    {
      return new EditSmoothingTimeView();
    }

    Control *createEditor() override
    {
      return new EditSmoothingTimeEditor();
    }
  };

  struct PedalSettings : InnerNode
  {
    explicit PedalSettings(InnerNode *parent)
        : InnerNode(parent, "Pedals")
    {
      children.emplace_back(new PedalSetting(this, HardwareSourcesGroup::getPedal1ParameterID().getNumber()));
      children.emplace_back(new PedalSetting(this, HardwareSourcesGroup::getPedal2ParameterID().getNumber()));
      children.emplace_back(new PedalSetting(this, HardwareSourcesGroup::getPedal3ParameterID().getNumber()));
      children.emplace_back(new PedalSetting(this, HardwareSourcesGroup::getPedal4ParameterID().getNumber()));
    }
  };

  struct DeviceSettings : InnerNode
  {
    explicit DeviceSettings(InnerNode *parent)
        : InnerNode(parent, "Device Settings")
    {
      children.emplace_back(new EditSmoothingTime(this));
      children.emplace_back(new SettingItem<TuneReference>(this, "Tune Reference"));
      children.emplace_back(new SettingItem<TransitionTime>(this, "Transition Time"));
      children.emplace_back(new Velocity(this));
      children.emplace_back(new Aftertouch(this));
      children.emplace_back(new BenderCurveSetting(this));
      children.emplace_back(new PedalSettings(this));
      children.emplace_back(new EnumSettingItem<PresetGlitchSuppression>(this, "Preset Glitch Suppression"));
      children.emplace_back(new EnumSettingItem<SyncVoiceGroupsAcrossUIS>(this, "Sync Parts across UIs"));
      children.emplace_back(new WiFiSetting(this));
      children.emplace_back(new StoreInitSound(this));
      children.emplace_back(new ResetInitSound(this));
    }
  };

  struct DeviceName : EditableLeaf
  {
    explicit DeviceName(InnerNode *parent)
        : EditableLeaf(parent, "Device Name")
    {
    }

    Control *createView() override
    {
      return new DeviceNameView();
    }

    Control *createEditor() override
    {
      return nullptr;
    }

    bool onEditModeEntered() override
    {
      auto &boled = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
      boled.setOverlay(new RenameDeviceLayout(boled.getLayout()));
      return true;
    }
  };

  struct SSID : Leaf
  {
    explicit SSID(InnerNode *parent)
        : Leaf(parent, "SSID")
    {
    }

    Control *createView() override
    {
      return new SSIDView();
    }
  };

  struct Passphrase : EditableLeaf
  {
    explicit Passphrase(InnerNode *parent)
        : EditableLeaf(parent, "Passphrase")
    {
    }

    Control *createView() override
    {
      return new PassphraseView();
    }

    Control *createEditor() override
    {
      return new PassphraseEditor();
    }
  };

  struct UpdateAvailable : EditableLeaf
  {
    explicit UpdateAvailable(InnerNode *parent)
        : EditableLeaf(parent, "Update Available")
    {
    }

    Control *createView() override
    {
      return new UpdateAvailableView();
    }

    Control *createEditor() override
    {
      if(UpdateAvailableView::updateExists())
        return new UpdateAvailableEditor();

      return nullptr;
    }
  };

  struct FreeInternalMemory : Leaf
  {
    explicit FreeInternalMemory(InnerNode *parent)
        : Leaf(parent, "Free Disk Space")
    {
    }

    Control *createView() override
    {
      return new FreeInternalMemoryView();
    }
  };

  struct UISoftwareVersion : EditableLeaf
  {
    explicit UISoftwareVersion(InnerNode *parent)
        : EditableLeaf(parent, "Software Version")
    {
    }

    Control *createView() override
    {
      auto info = Application::get().getDeviceInformation()->getItem<::SoftwareVersion>().get();
      return new DeviceInfoItemView(info);
    }

    Control *createEditor() override
    {
      return new UISoftwareVersionEditor();
    }
  };

  struct UniqueHardwareID : Leaf
  {
    explicit UniqueHardwareID(InnerNode *parent)
        : Leaf(parent, "Device Hardware ID ")
    {
    }

    Control *createView() override
    {
      auto info = Application::get().getDeviceInformation()->getItem<::UniqueHardwareID>().get();
      return new DeviceInfoItemView(info);
    }
  };

  struct DateTime : EditableLeaf
  {
    explicit DateTime(InnerNode *parent)
        : EditableLeaf(parent, "Date / Time")
    {
    }

    Control *createView() override
    {
      auto info = Application::get().getDeviceInformation()->getItem<::DateTimeInfo>().get();
      return new DeviceInfoItemView(info, std::chrono::milliseconds(500));
    }

    Control *createEditor() override
    {
      return new DateTimeEditor();
    }
  };

  struct WebUIAdress : Leaf
  {
    struct AddressLabel : public SetupLabel
    {
      AddressLabel()
          : SetupLabel("192.168.8.2", Rect(0, 0, 0, 0))
      {
      }
    };
    explicit WebUIAdress(InnerNode *parent)
        : Leaf(parent, "Website Address:")
    {
    }
    Control *createView() override
    {
      return new AddressLabel();
    }
  };

  struct RamUsage : Leaf
  {
    struct RamUsageLabel : public SetupLabel
    {
      RamUsageLabel()
          : SetupLabel("", Rect(0, 0, 0, 0))
      {
        Application::get().getSettings()->getSetting<UsedRAM>()->onChange(
            sigc::mem_fun(this, &RamUsageLabel::onSettingChanged));
      }

      void onSettingChanged(const Setting *s)
      {
        if(auto used = dynamic_cast<const UsedRAM *>(s))
        {
          auto settings = Application::get().getSettings();
          auto total = settings->getSetting<TotalRAM>();
          StringAndSuffix str { used->getDisplayString() + " / " + total->getDisplayString() + " MB", 0 };
          setText(str);
        }
      }
    };

    explicit RamUsage(InnerNode *parent)
        : Leaf(parent, "RAM Usage:")
    {
    }

    Control *createView() override
    {
      return new RamUsageLabel();
    }
  };

  struct BufferUnderrunsNode : Leaf
  {
    struct BufferUnderrunsLabel : public SetupLabel
    {
      BufferUnderrunsLabel()
          : SetupLabel("", Rect(0, 0, 0, 0))
      {
        Application::get().getDeviceInformation()->getItem<BufferUnderruns>()->onChange(
            sigc::mem_fun(this, &BufferUnderrunsLabel::onSettingChanged));
      }

      void onSettingChanged(const DeviceInformationItem *s)
      {
        if(auto used = dynamic_cast<const BufferUnderruns *>(s))
          setText(used->getDisplayString());
      }
    };

    explicit BufferUnderrunsNode(InnerNode *parent)
        : Leaf(parent, "Buffer Underruns:")
    {
    }

    Control *createView() override
    {
      return new BufferUnderrunsLabel();
    }
  };

  struct SystemInfo : InnerNode
  {
    explicit SystemInfo(InnerNode *parent)
        : InnerNode(parent, "System Info")
    {
      children.emplace_back(new DeviceName(this));
      children.emplace_back(new SSID(this));
      children.emplace_back(new Passphrase(this));
      children.emplace_back(new WebUIAdress(this));
      children.emplace_back(new DateTime(this));
      children.emplace_back(new FreeInternalMemory(this));
      children.emplace_back(new RamUsage(this));
      children.emplace_back(new UISoftwareVersion(this));
      children.emplace_back(new UniqueHardwareID(this));
      children.emplace_back(new UpdateAvailable(this));
      children.emplace_back(new BufferUnderrunsNode(this));
    }
  };

  struct About : EditableLeaf
  {
    explicit About(InnerNode *parent)
        : EditableLeaf(parent, "About")
    {
    }

    Control *createView() override
    {
      return new SetupLabel("...", Rect(0, 0, 0, 0));
    }

    Control *createEditor() override
    {
      return nullptr;
    }

    bool onEditModeEntered() override
    {
      auto &boled = Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled();
      boled.setOverlay(new AboutLayout());
      return true;
    }
  };

  struct EncoderAcceleration : EditableLeaf
  {
    explicit EncoderAcceleration(InnerNode *parent)
        : EditableLeaf(parent, "Encoder Acceleration")
    {
    }

    Control *createView() override
    {
      return new EncoderAccelerationView();
    }

    Control *createEditor() override
    {
      return new EncoderAccelerationEditor();
    }
  };

  struct RibbonRelativeFactorSetting : EditableLeaf
  {
    explicit RibbonRelativeFactorSetting(InnerNode *parent)
        : EditableLeaf(parent, "Ribbon Relative Factor")
    {
    }

    Control *createView() override
    {
      return new RibbonRelativeFactorSettingView();
    }

    Control *createEditor() override
    {
      return new RibbonRelativeFactorSettingEditor();
    }
  };

  struct ScreenSaverTime : EditableLeaf
  {
    explicit ScreenSaverTime(InnerNode *parent)
        : EditableLeaf(parent, "Screensaver Timeout")
    {
    }

    Control *createView() override
    {
      return new ScreenSaverTimeView();
    }

    Control *createEditor() override
    {
      return new ScreenSaverTimeEditor();
    }
  };

  struct HardwareUI : InnerNode
  {
    explicit HardwareUI(InnerNode *parent)
        : InnerNode(parent, "Hardware UI")
    {
      children.emplace_back(new EncoderAcceleration(this));
      children.emplace_back(new RibbonRelativeFactorSetting(this));
      children.emplace_back(new EnumSettingItem<SignalFlowIndicationSetting>(this, "Signal Flow Indication"));
      children.emplace_back(new ScreenSaverTime(this));
    }
  };

  struct USBStickAvailable : Leaf
  {
    explicit USBStickAvailable(InnerNode *parent)
        : Leaf(parent, "USB Available")
    {
    }

    Control *createView() override
    {
      return new USBStickAvailableView();
    }
  };

  struct BackupExport : EditableLeaf
  {
    explicit BackupExport(InnerNode *parent)
        : EditableLeaf(parent, "Save all Banks...")
    {
    }

    Control *createView() override
    {
      return new ExportBackupView();
    }

    Control *createEditor() override
    {
      return new ExportBackupEditor();
    }
  };

  struct BackupImport : EditableLeaf
  {
    explicit BackupImport(InnerNode *parent)
        : EditableLeaf(parent, "Restore all Banks...")
    {
    }

    Control *createView() override
    {
      return new ImportBackupView();
    }

    Control *createEditor() override
    {
      return new ImportBackupEditor();
    }
  };

  struct Backup : InnerNode
  {
    explicit Backup(InnerNode *parent)
        : InnerNode(parent, "Backup")
    {
      children.emplace_back(new USBStickAvailable(this));
      children.emplace_back(new BackupExport(this));
      children.emplace_back(new BackupImport(this));
    }
  };

  struct SettingView : SetupLabel
  {
    SettingView()
        : SetupLabel("...", Rect(0, 0, 0, 0))
    {
      setText("...");
    }
  };

  struct MidiChannels : InnerNode
  {
    explicit MidiChannels(InnerNode *parent)
        : InnerNode(parent, "Channels")
    {
      children.emplace_back(new EnumSettingItem<MidiSendChannelSetting>(this, "Send Channel"));
      children.emplace_back(new EnumSettingItem<MidiSendChannelSplitSetting>(this, "Send Split Channel"));
      children.emplace_back(new EnumSettingItem<MidiReceiveChannelSetting>(this, "Receive Channel"));
      children.emplace_back(new EnumSettingItem<MidiReceiveChannelSplitSetting>(this, "Receive Split Channel"));
    }
  };

  struct ResetMidiSettingsToHighRes : public OneShotEntry
  {

    explicit ResetMidiSettingsToHighRes(InnerNode *parent)
        : OneShotEntry(parent, "Set to High-Res. Defaults", OneShotTypes::StartCB([]() {
          SettingsUseCases useCases(*Application::get().getSettings());
          useCases.setMappingsToHighRes();
        }))
    {
    }
  };

  struct ResetMidiSettingsToClassic : public OneShotEntry
  {

    explicit ResetMidiSettingsToClassic(InnerNode *parent)
        : OneShotEntry(parent, "Set to Classic MIDI Defaults", OneShotTypes::StartCB([]() {
          SettingsUseCases useCases(*Application::get().getSettings());
          useCases.setMappingsToClassicMidi();
        }))
    {
    }
  };

  struct MidiAssignments : InnerNode
  {
    explicit MidiAssignments(InnerNode *parent)
        : InnerNode { parent, "Assignments" }
    {
      children.emplace_back(new EnumSettingItem<PedalCCMapping<1>>(this, "Pedal 1"));
      children.emplace_back(new EnumSettingItem<PedalCCMapping<2>>(this, "Pedal 2"));
      children.emplace_back(new EnumSettingItem<PedalCCMapping<3>>(this, "Pedal 3"));
      children.emplace_back(new EnumSettingItem<PedalCCMapping<4>>(this, "Pedal 4"));
      children.emplace_back(new EnumSettingItem<RibbonCCMapping<1>>(this, "Ribbon 1"));
      children.emplace_back(new EnumSettingItem<RibbonCCMapping<2>>(this, "Ribbon 2"));
      children.emplace_back(new EnumSettingItem<BenderCCMapping>(this, "Bender"));
      children.emplace_back(new EnumSettingItem<AftertouchCCMapping>(this, "Aftertouch"));
      children.emplace_back(new EnumSettingItem<Enable14BitSupport>(this, "High-Res. CCs (use LSB)"));
      children.emplace_back(new EnumSettingItem<EnableHighVelocityCC>(this, "High-Res. Velocity (CC 88)"));
      children.emplace_back(new ResetMidiSettingsToClassic(this));
      children.emplace_back(new ResetMidiSettingsToHighRes(this));
    }
  };

  struct MidiProgramChangeBank : EditableLeaf
  {
   private:
    struct View : SetupLabel
    {
      View()
          : SetupLabel({ 0, 0, 0, 0 })
      {
      }

      StringAndSuffix getText() const override
      {
        auto pm = Application::get().getPresetManager();
        auto selected = pm->getMidiSelectedBank();
        auto selectedBank = pm->findBank(selected);

        if(selectedBank)
        {
          auto pos = pm->getBankPosition(selected);
          return { std::to_string(pos + 1) + "-" + selectedBank->getName(true), 0 };
        }
        else
        {
          return { "none", 0 };
        }
      }
    };

    struct Editor : MenuEditor
    {
     private:
      static auto getPresetManager()
      {
        return Application::get().getPresetManager();
      }

      static auto getMidiBank()
      {
        return getPresetManager()->findMidiSelectedBank();
      }

     public:
      Editor()
          : MenuEditor()
      {
        getPresetManager()->onMidiBankSelectionHappened(
            sigc::hide(sigc::mem_fun(this, &Editor::updateOnSettingChanged)));
      }

     protected:
      void incSetting(int inc) override
      {
        auto pm = getPresetManager();
        auto settings = Application::get().getSettings();
        PresetManagerUseCases useCase(*pm, *settings);
        const auto numBanks = pm->getNumBanks();

        if(getMidiBank())
        {
          auto currentIndex = getSelectedIndex();
          auto newIndex = currentIndex + inc;
          auto size = getDisplayStrings().size();

          newIndex %= static_cast<int>(size);

          if(newIndex != 0)
            useCase.selectMidiBank(pm->getBankAt(newIndex - 1));
          else
            useCase.selectMidiBank(nullptr);
        }
        else if(numBanks > 0)
        {
          if(inc > 0)
            useCase.selectMidiBank(pm->getBankAt(0));
          else if(inc < 0)
            useCase.selectMidiBank(pm->getBankAt(pm->getNumBanks() - 1));
        }
      }

      const std::vector<Glib::ustring> &getDisplayStrings() const override
      {
        static std::vector<Glib::ustring> sBankStrings;
        auto pm = Application::get().getPresetManager();
        int displayIndex = 1;
        sBankStrings.clear();
        sBankStrings.emplace_back("None");
        for(auto b : pm->getBanks())
        {
          sBankStrings.emplace_back(std::to_string(displayIndex) + "-" + b->getName(true));
          displayIndex++;
        }
        return sBankStrings;
      }

      int getSelectedIndex() const override
      {
        if(auto sel = getMidiBank())
          return static_cast<int>(getPresetManager()->getBankPosition(sel->getUuid()) + 1);
        else
          return 0;
      }
    };

   public:
    explicit MidiProgramChangeBank(InnerNode *n)
        : EditableLeaf(n, "Program Change Bank \uE0C1")
    {
    }

    Control *createView() override
    {
      return new View();
    }

    Control *createEditor() override
    {
      return new Editor();
    }
  };

  struct MidiPanicButton : OneShotEntry
  {
    explicit MidiPanicButton(InnerNode *p)
        : OneShotEntry(p, "Panic", OneShotTypes::StartCB([]() { SettingsUseCases::panicAudioEngine(); }))
    {
    }
  };

  struct RecorderStopButton : OneShotEntry
  {
    explicit RecorderStopButton(InnerNode* p)
    : OneShotEntry(p, "Stop Playback", OneShotTypes::StartCB([]{ RecorderManager::stopRecorderPlayback(); }))
    {
    }
  };

  struct RoutingsEntry : public EditableLeaf
  {
   public:
    RoutingsEntry(RoutingSettings::tRoutingIndex id, InnerNode *p, const Glib::ustring &text, RoutingSettings::tRoutingIndex& parentSelection)
        : EditableLeaf(p, text)
        , m_id { id }
        , m_parentSelection { parentSelection }
    {
    }

    Control *createView() override
    {
      return new RoutingsView(m_id);
    }

    Control *createEditor() override
    {
      return new RoutingsEditor(m_id, m_parentSelection);
    }

   private:
    const RoutingSettings::tRoutingIndex m_id;
    RoutingSettings::tRoutingIndex &m_parentSelection;
  };

  template <bool value> struct SetRoutingsTo : public OneShotEntry
  {

    explicit SetRoutingsTo(InnerNode *parent)
        : OneShotEntry(parent, getName(), OneShotTypes::StartCB([]() {
          SettingsUseCases useCases(*Application::get().getSettings());
          useCases.setAllRoutingEntries(value);
        }))
    {
    }

    constexpr const char *getName()
    {
      if constexpr(value)
        return "Set all Routings to On";
      else
        return "Set all Routings to Off";
    }
  };

  struct MidiRoutings : InnerNode
  {
    typedef RoutingSettings::tRoutingIndex tID;
    explicit MidiRoutings(InnerNode *p)
        : InnerNode(p, "Routings")
    {
      children.emplace_back(new RoutingsEntry(tID::Notes, this, "Notes", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::ProgramChange, this, "Prog. Ch.", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Pedal1, this, "Pedal 1", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Pedal2, this, "Pedal 2", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Pedal3, this, "Pedal 3", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Pedal4, this, "Pedal 4", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Bender, this, "Bender", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Aftertouch, this, "Aftertouch", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Ribbon1, this, "Ribbon 1", m_selectedRouting));
      children.emplace_back(new RoutingsEntry(tID::Ribbon2, this, "Ribbon 2", m_selectedRouting));
      children.emplace_back(new SetRoutingsTo<true>(this));
      children.emplace_back(new SetRoutingsTo<false>(this));
    }

    Node *getDesiredFocusChangeOnEditModeExited() override
    {
      auto at = [](auto &list, auto n) {
        auto it = list.begin();
        std::advance(it, n);
        return it->get();
      };

      switch(m_selectedRouting)
      {
        case tID::Pedal1:
          return at(children, 2);
        case tID::Pedal2:
          return at(children, 3);
        case tID::Pedal3:
          return at(children, 4);
        case tID::Pedal4:
          return at(children, 5);
        case tID::Bender:
          return at(children, 6);
        case tID::Aftertouch:
          return at(children, 7);
        case tID::Ribbon1:
          return at(children, 8);
        case tID::Ribbon2:
          return at(children, 9);
        case tID::ProgramChange:
          return at(children, 1);
        case tID::Notes:
          return at(children, 0);
        case tID::LENGTH:
          break;
      }
      return nullptr;
    }

    Control *createView() override
    {
      return new SetupLabel("...", Rect(0, 0, 0, 0));
    }

   private:
    tID m_selectedRouting = tID::Notes;
  };

  struct MidiSettings : InnerNode
  {
    explicit MidiSettings(InnerNode *parent)
        : InnerNode(parent, "MIDI Settings")
    {
      children.emplace_back(new MidiPanicButton(this));
      children.emplace_back(new EnumSettingItem<GlobalLocalEnableSetting>(this, "Local Enable"));
      children.emplace_back(new MidiProgramChangeBank(this));
      children.emplace_back(new MidiChannels(this));
      children.emplace_back(new MidiAssignments(this));
      children.emplace_back(new MidiRoutings(this));
    }
  };

  struct FlacSettings : InnerNode
  {
    explicit FlacSettings(InnerNode *parent)
        : InnerNode(parent, "Recorder Settings")
    {
      children.emplace_back(new EnumSettingItem<AutoStartRecorderSetting>(this, "Auto-Start Recording"));
      children.emplace_back(new RecorderStopButton(this));
    }
  };

  struct Setup : InnerNode
  {
    Setup()
        : InnerNode(nullptr, "Setup")
    {
      children.emplace_back(new DeviceSettings(this));
      children.emplace_back(new HardwareUI(this));
      children.emplace_back(new MidiSettings(this));
      children.emplace_back(new FlacSettings(this));
      children.emplace_back(new SystemInfo(this));
      children.emplace_back(new About(this));
      children.emplace_back(new Backup(this));
      focus = children.begin();
    }

    [[nodiscard]] Glib::ustring getName() const override
    {
      return "Setup";
    }

    bool diveInto()
    {
      if(auto e = dynamic_cast<InnerNode *>((*focus).get()))
      {
        focus = e->children.begin();
        return true;
      }
      return false;
    }

    void diveOutOfEditMode()
    {
      auto focusNode = (*focus).get();

      if(auto pa = focusNode->parent)
      {
        if(auto newFocus = pa->getDesiredFocusChangeOnEditModeExited())
        {
          for(auto it = pa->children.begin(); it != pa->children.end(); it++)
          {
            if(it->get() == newFocus)
            {
              focus = it;
              return;
            }
          }
        }
      }
    }

    bool diveUp()
    {
      auto focusNode = (*focus).get();

      if(focusNode->parent && focusNode->parent->parent)
      {
        auto pa = focusNode->parent;
        auto grandpa = focusNode->parent->parent;
        for(auto it = grandpa->children.begin(); it != grandpa->children.end(); it++)
        {
          if(pa == it->get())
          {
            focus = it;
            return true;
          }
        }
      }
      return false;
    }

    void incFocus(int i)
    {
      auto p = (*focus)->parent;

      while(i > 0 && focus != p->children.end())
      {
        if(++focus == p->children.end())
          --focus;
        i--;
      }

      while(i < 0 && focus != p->children.begin())
      {
        focus--;
        i++;
      }
    }

    std::list<std::unique_ptr<Node>>::iterator focus;
  };
}

class Breadcrumb : public Control
{
 public:
  explicit Breadcrumb(NavTree::Node *node)
      : Control(Rect(0, 0, 256, 12))
      , m_node(node)
  {
  }

  bool redraw(FrameBuffer &fb) override
  {
    fb.setColor(FrameBufferColors::C103);
    fb.fillRect(getPosition());
    drawNodeRecursivly(fb, m_node);
    return true;
  }

  int drawNodeRecursivly(FrameBuffer &fb, NavTree::Node *node)
  {
    if(node)
    {
      auto left = drawNodeRecursivly(fb, node->parent);
      auto isTip = node == m_node;
      auto title = node->getName();

      if(isTip)
      {
        fb.setColor(FrameBufferColors::C255);
      }
      else
      {
        fb.setColor(FrameBufferColors::C179);
        title = title + " > ";
      }

      auto font = Fonts::get().getFont("Emphase-9-Regular", 9);
      auto width = font->draw(fb, title, left, getPosition().getBottom() - 1);
      return left + width;
    }
    return 5;
  }

 private:
  NavTree::Node *m_node;
};

SetupLayout::SetupLayout(FocusAndMode focusAndMode)
    : super(Application::get().getHWUI()->getPanelUnit().getEditPanel().getBoled())
    , m_tree(new NavTree::Setup())
    , m_focusAndMode(focusAndMode)
{
  buildPage();
}

SetupLayout::~SetupLayout() = default;

void SetupLayout::addBreadcrumb()
{
  auto focus = m_tree->focus->get();

  if(isInEditMode())
    addControl(new Breadcrumb(dynamic_cast<NavTree::Leaf *>(focus)));
  else
    addControl(new Breadcrumb(focus->parent));
}

void SetupLayout::buildPage()
{
  clear();
  m_editor = nullptr;

  addBreadcrumb();
  addSelectionEntries();

  if(!addEditor())
    addValueViews();

  finishLists();
}

void SetupLayout::addSelectionEntries()
{
  auto focus = m_tree->focus->get();
  auto focusParent = focus->parent;

  for(auto &c : focusParent->children)
  {
    Control *s = c->createSelectionControl();
    s->setHighlight((c.get() == focus));
    addSelectionEntry(s);
  }
}

void SetupLayout::addValueViews()
{
  auto focus = m_tree->focus->get();
  auto focusParent = focus->parent;

  for(auto &c : focusParent->children)
  {
    bool isFocussed = c.get() == focus;
    addView(c->createView(), isFocussed);
  }
}

bool SetupLayout::isInEditMode() const
{
  auto focus = m_tree->focus->get();
  auto focusEditable = dynamic_cast<NavTree::Leaf *>(focus);
  return m_focusAndMode.mode == UIMode::Edit && focusEditable;
}

bool SetupLayout::addEditor()
{
  if(isInEditMode())
  {
    auto focus = m_tree->focus->get();

    if(auto leaf = dynamic_cast<NavTree::EditableLeaf *>(focus))
    {
      if(auto e = leaf->createEditor())
      {
        setEditor(e);
        return true;
      }
      else
      {
        m_focusAndMode.mode = UIMode::Select;
      }
    }
  }
  return false;
}

void SetupLayout::diveUp()
{
  if(m_focusAndMode.mode == UIMode::Edit)
  {
    m_focusAndMode.mode = UIMode::Select;
    m_tree->diveOutOfEditMode();
  }
  else
    m_tree->diveUp();

  buildPage();
}

bool SetupLayout::onButton(Buttons i, bool down, ButtonModifiers modifiers)
{
  SettingsUseCases useCases(*Application::get().getSettings());

  if(down)
  {
    if(i == Buttons::BUTTON_PRESET)
    {
      useCases.setFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Select));
      return true;
    }
    else if(i == Buttons::BUTTON_STORE)
    {
      useCases.setFocusAndMode(FocusAndMode(UIFocus::Presets, UIMode::Store));
      return true;
    }

    if(m_focusAndMode.mode == UIMode::Select)
    {
      if(i == Buttons::BUTTON_ENTER || i == Buttons::BUTTON_C || i == Buttons::BUTTON_D)
      {
        onEnterInSelectionMode(modifiers);
        return true;
      }
    }
    else if(m_editor)
    {
      if(!m_editor->onButton(i, down, modifiers))
      {
        if(i == Buttons::BUTTON_ENTER)
        {
          onEnterInEditMode();
          return true;
        }
      }
      else if(i == Buttons::BUTTON_B)
      {
        return true;
      }
    }

    if(i == Buttons::BUTTON_A || i == Buttons::BUTTON_B)
    {
      diveUp();
      return true;
    }
  }

  return super::onButton(i, down, modifiers);
}

void SetupLayout::onEnterInSelectionMode(ButtonModifiers modifiers)
{
  bool diveable = dynamic_cast<NavTree::EditableLeaf *>(m_tree->focus->get());
  diveable |= dynamic_cast<NavTree::InnerNode *>(m_tree->focus->get()) != nullptr;

  if(diveable)
  {
    if(!m_tree->diveInto())
    {
      m_focusAndMode.mode = UIMode::Edit;

      if(m_tree->focus->get()->onEditModeEntered())
      {
        m_focusAndMode.mode = UIMode::Select;
        return;
      }
    }
  }

  buildPage();
}

void SetupLayout::onEnterInEditMode()
{
  m_focusAndMode.mode = UIMode::Select;
  buildPage();
}

bool SetupLayout::onRotary(int inc, ButtonModifiers modifiers)
{
  if(m_focusAndMode.mode == UIMode::Select)
  {
    m_tree->incFocus(inc);
    buildPage();
    return true;
  }
  else if(m_editor)
  {
    return m_editor->onRotary(inc, modifiers);
  }
  return super::onRotary(inc, modifiers);
}

void SetupLayout::addSelectionEntry(Control *s)
{
  ensureSelectionEntries()->addEntry(s);
}

void SetupLayout::addView(Control *c, bool focussed)
{
  ensureViewEntries()->addEntry(c, focussed);
}

void SetupLayout::setEditor(Control *c)
{
  addControl(c);
  c->setPosition(Rect(129, 28, 126, 12));
  m_editor = dynamic_cast<SetupEditor *>(c);
  m_editor->setSetupLayout(this);
  g_assert(m_editor);
}

void SetupLayout::finishLists()
{
  if(auto r = findControlOfType<SetupSelectionEntries>())
    r->finish(m_focusAndMode.mode == UIMode::Select);

  if(auto r = findControlOfType<ViewEntries>())
    r->finish();
}

std::shared_ptr<SetupSelectionEntries> SetupLayout::ensureSelectionEntries()
{
  if(auto r = findControlOfType<SetupSelectionEntries>())
    return r;

  addControl(new SetupSelectionEntries(Rect(1, 16, 126, 48)));
  return ensureSelectionEntries();
}

std::shared_ptr<ViewEntries> SetupLayout::ensureViewEntries()
{
  if(auto r = findControlOfType<ViewEntries>())
    return r;

  addControl(new ViewEntries(Rect(129, 16, 126, 48)));
  return ensureViewEntries();
}

bool SetupLayout::redraw(FrameBuffer &fb)
{
  ControlOwner::redraw(fb);

  if(m_focusAndMode.mode == UIMode::Select)
  {
    fb.setColor(FrameBufferColors::C179);
    fb.drawRect(Rect(0, 28, 256, 12));
  }
  return true;
}
