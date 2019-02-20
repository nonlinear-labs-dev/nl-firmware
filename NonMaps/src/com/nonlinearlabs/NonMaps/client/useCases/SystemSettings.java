package com.nonlinearlabs.NonMaps.client.useCases;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.AftertouchCurve;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BenderCurve;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.DebugLevel;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.PedalType;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.VelocityCurve;

public class SystemSettings {
	private static SystemSettings theInstance = new SystemSettings();

	public static SystemSettings get() {
		return theInstance;
	}

	public void setSendPresetAsLPCWriteFallback(BooleanValues v) {
		Setup.get().systemSettings.sendPresetAsLPCFallback.setValue(v);
		NonMaps.get().getServerProxy().setSetting("SendPresetAsLPCWriteFallback", netify(v.name()));
	}

	private String netify(String name) {
		name = name.toLowerCase();
		return name.replace("_", "-");
	}

	public void setDebugLevel(DebugLevel v) {
		Setup.get().systemSettings.debugLevel.setValue(v);
		NonMaps.get().getServerProxy().setSetting("DebugLevel", netify(v.name()));
	}

	public void setKioskMode(BooleanValues v) {
		Setup.get().systemSettings.kioskMode.setValue(v);
		NonMaps.get().getServerProxy().setSetting("KioskMode", netify(v.name()));
	}

	public void setBlockingMainThreadIndication(BooleanValues v) {
		Setup.get().systemSettings.indicateBlockedUI.setValue(v);
		NonMaps.get().getServerProxy().setSetting("IndicateBlockedUI", netify(v.name()));

	}

	public void setDeviceName(String newName) {
		Setup.get().systemSettings.deviceName.setValue(newName);
		NonMaps.theMaps.getServerProxy().setSetting("DeviceName", newName);
	}

	public void setEditSmoothingTime(double v) {
		Setup.get().systemSettings.editSmoothingTime.value.setValue(v);
		NonMaps.theMaps.getServerProxy().setSetting("EditSmoothingTime", Double.toString(v));
	}

	public void setPitchBenderRaw(BooleanValues v) {
		Setup.get().systemSettings.benderRampBypass.setValue(v);
		NonMaps.get().getServerProxy().setBenderRampBypass(v);
	}

	public void setVelocityCurve(VelocityCurve c) {
		Setup.get().systemSettings.velocityCurve.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("VelocityCurve", netify(c.name()));
	}

	public void setAftertouchCurve(AftertouchCurve c) {
		Setup.get().systemSettings.aftertouchCurve.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("AftertouchCurve", netify(c.name()));
	}

	public void setBenderCurve(BenderCurve c) {
		Setup.get().systemSettings.benderCurve.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("BenderCurve", netify(c.name()));
	}

	public void setPedal1Type(PedalType c) {
		Setup.get().systemSettings.pedal1Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal1Type", netify(c.name()));
	}

	public void setPedal2Type(PedalType c) {
		Setup.get().systemSettings.pedal2Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal2Type", netify(c.name()));
	}

	public void setPedal3Type(PedalType c) {
		Setup.get().systemSettings.pedal3Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal3Type", netify(c.name()));
	}

	public void setPedal4Type(PedalType c) {
		Setup.get().systemSettings.pedal4Type.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("Pedal4Type", netify(c.name()));
	}

	public void setPresetGlitchSurpression(BooleanValues c) {
		Setup.get().systemSettings.presetGlitchSuppression.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("PresetGlitchSuppression", netify(c.name()));
	}

	public void setHighlightChangedParameters(BooleanValues c) {
		Setup.get().systemSettings.highlightChangedParameters.setValue(c);
		NonMaps.theMaps.getServerProxy().setSetting("HighlightChangedParameters", netify(c.name()));
	}
}
