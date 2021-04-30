package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.AftertouchCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.AftertouchCurve;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BenderCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BenderCurve;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.DebugLevel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiReceiveChannel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiReceiveChannelSplit;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiSendChannel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.MidiSendChannelSplit;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.PedalCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.PedalType;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.RibbonCCMapping;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.VelocityCurve;

public class SystemSettings {
	private static SystemSettings theInstance = new SystemSettings();

	public static SystemSettings get() {
		return theInstance;
	}

	public void setSendPresetAsPlaycontrollerWriteFallback(BooleanValues v) {
		SetupModel.get().systemSettings.sendPresetAsPlaycontrollerFallback.setValue(v);
		NonMaps.get().getServerProxy().setSetting("SendPresetAsLPCWriteFallback", netify(v.name()));
	}

	private String netify(String name) {
		name = name.toLowerCase();
		return name.replace("_", "-");
	}

	public void setDebugLevel(DebugLevel v) {
		SetupModel.get().systemSettings.debugLevel.setValue(v);
		NonMaps.get().getServerProxy().setSetting("DebugLevel", netify(v.name()));
	}

	public void setKioskMode(BooleanValues v) {
		SetupModel.get().systemSettings.kioskMode.setValue(v);
		NonMaps.get().getServerProxy().setSetting("KioskMode", netify(v.name()));
	}

	public void setBlockingMainThreadIndication(BooleanValues v) {
		SetupModel.get().systemSettings.indicateBlockedUI.setValue(v);
		NonMaps.get().getServerProxy().setSetting("IndicateBlockedUI", netify(v.name()));
	}

	public void setDeviceName(String newName) {
		SetupModel.get().systemSettings.deviceName.setValue(newName);
		NonMaps.theMaps.getServerProxy().setSetting("DeviceName", newName);
	}

	public void setEditSmoothingTime(double v) {
		SetupModel.get().systemSettings.editSmoothingTime.value.setValue(v);
		NonMaps.theMaps.getServerProxy().setSetting("EditSmoothingTime", Double.toString(v));
	}

	public void setVelocityCurve(VelocityCurve c) {
		SetupModel.get().systemSettings.velocityCurve.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("VelocityCurve", netify(c.name()));
	}

	public void setSyncParts(BooleanValues v) {
		SetupModel.get().systemSettings.syncVoiceGroups.setValue(v);
		NonMaps.theMaps.getServerProxy().setSetting("SyncVoiceGroups", netify(v.name()));
	}

	public void setAftertouchCurve(AftertouchCurve c) {
		SetupModel.get().systemSettings.aftertouchCurve.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("AftertouchCurve", netify(c.name()));
	}

	public void setBenderCurve(BenderCurve c) {
		SetupModel.get().systemSettings.benderCurve.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("BenderCurve", netify(c.name()));
	}

	public void setPedal1Type(PedalType c) {
		SetupModel.get().systemSettings.pedal1Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal1Type", c.name());
	}

	public void setPedal2Type(PedalType c) {
		SetupModel.get().systemSettings.pedal2Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal2Type", c.name());
	}

	public void setPedal3Type(PedalType c) {
		SetupModel.get().systemSettings.pedal3Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal3Type", c.name());
	}

	public void setPedal4Type(PedalType c) {
		SetupModel.get().systemSettings.pedal4Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal4Type", c.name());
	}

	public void setPresetGlitchSuppression(BooleanValues c) {
		SetupModel.get().systemSettings.presetGlitchSuppression.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("PresetGlitchSuppression", netify(c.name()));
	}

	public void setHighlightChangedParameters(BooleanValues c) {
		SetupModel.get().systemSettings.highlightChangedParameters.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("HighlightChangedParameters", netify(c.name()));
	}

	public void setCrashOnError(BooleanValues c) {
		SetupModel.get().systemSettings.crashOnError.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("CrashOnError", netify(c.name()));
	}

	public void setRandomizeAmount(double amount) {
		SetupModel.get().systemSettings.randomizeAmount.value.setValue(amount);
		NonMaps.theMaps.getServerProxy().setRandomAmount(amount);
	}

	public void setTransitionTime(double v) {
		SetupModel.get().systemSettings.transitionTime.value.setValue(v);
		NonMaps.theMaps.getServerProxy().setTransitionTime(v);
	}

	public void setTuneReference(double v) {
		SetupModel.get().systemSettings.tuneReference.value.setValue(v);
		NonMaps.theMaps.getServerProxy().setSetting("TuneReference", Double.toString(v));
	}

	public void resetInitSound() {
		NonMaps.theMaps.getServerProxy().resetInit();
	}

	public void storeInitSound() {
		NonMaps.theMaps.getServerProxy().storeAsInit();
	}

	public void setReceiveMidiChannel(MidiReceiveChannel midiReceiveChannel) {
		SetupModel.get().systemSettings.receiveChannel.setValue(midiReceiveChannel);
		NonMaps.theMaps.getServerProxy().setSetting("ReceiveChannel", midiReceiveChannel.name());
	}

	public void setPedal1Mapping(PedalCCMapping value) {
		SetupModel.get().systemSettings.pedal1Mapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal1Mapping", value.name());
	}

	public void setPedal2Mapping(PedalCCMapping value) {
		SetupModel.get().systemSettings.pedal2Mapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal2Mapping", value.name());
	}

	public void setPedal3Mapping(PedalCCMapping value) {
		SetupModel.get().systemSettings.pedal3Mapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal3Mapping", value.name());
	}

	public void setPedal4Mapping(PedalCCMapping value) {
		SetupModel.get().systemSettings.pedal4Mapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal4Mapping", value.name());
	}

	public void setRibbon1Mapping(RibbonCCMapping value) {
		SetupModel.get().systemSettings.ribbon1Mapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("Ribbon1Mapping", value.name());
	}

	public void setRibbon2Mapping(RibbonCCMapping value) {
		SetupModel.get().systemSettings.ribbon2Mapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("Ribbon2Mapping", value.name());
	}

	public void setAftertouchMapping(AftertouchCCMapping value) {
		SetupModel.get().systemSettings.aftertouchMapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("AftertouchMapping", value.name());
	}

	public void setPitchbendMapping(BenderCCMapping value) {
		SetupModel.get().systemSettings.benderMapping.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("BenderMapping", value.name());
	}

	public void setHighVelocityCC(BooleanValues b) {
		SetupModel.get().systemSettings.highVelocityCC.setValue(b);
		NonMaps.theMaps.getServerProxy().setSetting("HighVeloCC", netify(b.name()));
	}

	public void set14BitSupport(BooleanValues b)
	{
		SetupModel.get().systemSettings.enable14BitSupport.setValue(b);
		NonMaps.theMaps.getServerProxy().setSetting("HighResCC", netify(b.name()));
	}

	public void setReceiveMidiChannelSplit(MidiReceiveChannelSplit midiReceiveChannelSplit) {
		SetupModel.get().systemSettings.receiveChannelSplit.setValue(midiReceiveChannelSplit);
		NonMaps.theMaps.getServerProxy().setSetting("ReceiveChannelSplit", midiReceiveChannelSplit.name());
	}

	public void setReceiveNotes(BooleanValues enabled) {
		SetupModel.get().systemSettings.receiveNotes.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("ReceiveNotes", netify(enabled.name()));
	}

	public void setReceiveProgramChanges(BooleanValues enabled) {
		SetupModel.get().systemSettings.receiveProgramChanges.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("ReceiveProgramChanges", netify(enabled.name()));
	}

	public void setReceiveControllers(BooleanValues enabled) {
		SetupModel.get().systemSettings.receiveControllers.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("ReceiveControllers", netify(enabled.name()));
	}

	public void setReceiveAftertouchCurve(AftertouchCurve value) {
		SetupModel.get().systemSettings.receiveAftertouchCurve.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("ReceiveAftertouchCurve", value.name());
	}

	public void setReceiveVelocityCurve(VelocityCurve value) {
		SetupModel.get().systemSettings.receiveVelocityCurve.setValue(value);
		NonMaps.theMaps.getServerProxy().setSetting("ReceiveVelocityCurve", value.name());
	}

	public void setSendChannel(MidiSendChannel channel) {
		SetupModel.get().systemSettings.sendChannel.setValue(channel);
		NonMaps.theMaps.getServerProxy().setSetting("SendChannel", channel.name());
	}

	public void setSendChannelSplit(MidiSendChannelSplit channel) {
		SetupModel.get().systemSettings.sendChannelSplit.setValue(channel);
		NonMaps.theMaps.getServerProxy().setSetting("SendChannelSplit", channel.name());
	}

	public void setSendProgramChanges(BooleanValues enabled) {
		SetupModel.get().systemSettings.sendProgramChanges.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("SendProgramChanges", netify(enabled.name()));
	}

	public void setSendNotes(BooleanValues enabled) {
		SetupModel.get().systemSettings.sendNotes.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("SendNotes", netify(enabled.name()));
	}

	public void setSendControllers(BooleanValues enabled) {
		SetupModel.get().systemSettings.sendControllers.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("SendControllers", netify(enabled.name()));
	}

	public void setLocalControllers(BooleanValues enabled) {
		SetupModel.get().systemSettings.localControllers.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("LocalControllers", netify(enabled.name()));
	}

	public void setLocalNotes(BooleanValues enabled) {
		SetupModel.get().systemSettings.localNotes.setValue(enabled);
		NonMaps.theMaps.getServerProxy().setSetting("LocalNotes", netify(enabled.name()));
	}
}
