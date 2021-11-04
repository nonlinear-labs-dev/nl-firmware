package com.nonlinearlabs.client.useCases;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.DataModelEntityBase;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.DisplayScaling;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.EditParameter;

public class LocalSettings {
	private static LocalSettings theInstance = new LocalSettings();
	private LocalStorage storage = null;

	public static LocalSettings get() {
		return theInstance;
	}

	public void setStorage(LocalStorage s) {
		storage = s;

		load("SelectionAutoScroll", SetupModel.get().localSettings.selectionAutoScroll,
				SetupModel.SelectionAutoScroll.parameter_and_preset.name());
		load("ParameterDrag", SetupModel.get().localSettings.editParameter,
				SetupModel.EditParameter.if_selected.name());
		load("ContextMenus", SetupModel.get().localSettings.contextMenus, SetupModel.BooleanValues.on.name());
		load("PresetDragDropEnabled", SetupModel.get().localSettings.presetDragDrop,
				SetupModel.BooleanValues.on.name());
		load("UI Scaling", SetupModel.get().localSettings.displayScaling, SetupModel.DisplayScaling.percent_100.name());
		load("StripeBrightness", SetupModel.get().localSettings.stripeBrightness,
				SetupModel.StripeBrightness.percent_25.name());
		load("BitmapCache", SetupModel.get().localSettings.bitmapCache, SetupModel.BooleanValues.on.name());
		load("ShowDeveloperOptions", SetupModel.get().localSettings.showDeveloperOptions,
				SetupModel.BooleanValues.off.name());
		load("AlertOnBufferUnderruns", SetupModel.get().localSettings.alertOnBufferUnderruns,
				SetupModel.BooleanValues.off.name());
	}

	private void load(String key, DataModelEntityBase setting, String def) {
		String v = storage.load(key, def);
		v = convertFromLegacy(v);
		setting.fromString(v);
	}

	private String convertFromLegacy(String in) {
		in = in.replace("-", "_").toLowerCase();

		if (in.endsWith("%")) {
			in = "Percent_" + in.substring(0, in.length() - 1);
		}

		return in;
	}

	public void setSelectionAutoScroll(SetupModel.SelectionAutoScroll s) {
		SetupModel.get().localSettings.selectionAutoScroll.setValue(s);
		storage.store("SelectionAutoScroll", s.name());
	}

	public LocalStorage getStorage() {
		return storage;
	}

	public void setEditParameter(EditParameter e) {
		SetupModel.get().localSettings.editParameter.setValue(e);
		storage.store("ParameterDrag", e.name());
	}

	public void setContextMenus(BooleanValues v) {
		SetupModel.get().localSettings.contextMenus.setValue(v);
		storage.store("ContextMenus", v.name());
	}

	public void setPresetDragDrop(BooleanValues v) {
		SetupModel.get().localSettings.presetDragDrop.setValue(v);
		storage.store("PresetDragDropEnabled", v.name());
	}

	public void setDisplayScaling(DisplayScaling v) {
		SetupModel.get().localSettings.displayScaling.setValue(v);
		storage.store("UI Scaling", v.name());
	}

	public void setStripeBrightness(com.nonlinearlabs.client.dataModel.setup.SetupModel.StripeBrightness v) {
		SetupModel.get().localSettings.stripeBrightness.setValue(v);
		storage.store("StripeBrightness", v.name());
	}

	public void setBitmapCache(BooleanValues v) {
		SetupModel.get().localSettings.bitmapCache.setValue(v);
		storage.store("BitmapCache", v.name());
	}

	public void alertOnBufferUnderruns(BooleanValues v) {
		SetupModel.get().localSettings.alertOnBufferUnderruns.setValue(v);
		storage.store("AlertOnBufferUnderruns", v.name());
	}

	public void showDeveloperOptions(BooleanValues v) {
		boolean show = v == BooleanValues.on;

		if (show) {
			Window.alert("For developers only!");
		} else {
			Window.alert("Changes of the developer options will be discarded.");
			NonMaps.theMaps.getNonLinearWorld().getSettings().setToDefaults();
		}

		SetupModel.get().localSettings.showDeveloperOptions.setValue(v);
		storage.store("ShowDeveloperOptions", v.name());
	}
}
