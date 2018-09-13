package com.nonlinearlabs.NonMaps.client.useCases;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.DataModelEntityBase;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.DisplayScaling;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.EditParameter;

public class LocalSettings {
	private static LocalSettings theInstance = new LocalSettings();
	private LocalStorage storage = null;

	public static LocalSettings get() {
		return theInstance;
	}

	public void setStorage(LocalStorage s) {
		storage = s;

		load("SelectionAutoScroll", Setup.get().localSettings.selectionAutoScroll, Setup.SelectionAutoScroll.parameter_and_preset.name());
		load("ParameterDrag", Setup.get().localSettings.editParameter, Setup.EditParameter.if_selected.name());
		load("ContextMenus", Setup.get().localSettings.contextMenus, Setup.BooleanValues.on.name());
		load("PresetDragDropEnabled", Setup.get().localSettings.presetDragDrop, Setup.BooleanValues.on.name());
		load("UI Scaling", Setup.get().localSettings.displayScaling, Setup.DisplayScaling.percent_100.name());
		load("StripeBrightness", Setup.get().localSettings.stripeBrightness, Setup.StripeBrightness.percent_25.name());
		load("BitmapCache", Setup.get().localSettings.bitmapCache, Setup.BooleanValues.on.name());
		load("ShowDeveloperOptions", Setup.get().localSettings.showDeveloperOptions, Setup.BooleanValues.off.name());
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

	public void setSelectionAutoScroll(Setup.SelectionAutoScroll s) {
		Setup.get().localSettings.selectionAutoScroll.setValue(s);
		storage.store("SelectionAutoScroll", s.name());
	}

	public LocalStorage getStorage() {
		return storage;
	}

	public void setEditParameter(EditParameter e) {
		Setup.get().localSettings.editParameter.setValue(e);
		storage.store("ParameterDrag", e.name());
	}

	public void setContextMenus(BooleanValues v) {
		Setup.get().localSettings.contextMenus.setValue(v);
		storage.store("ContextMenus", v.name());
	}

	public void setPresetDragDrop(BooleanValues v) {
		Setup.get().localSettings.presetDragDrop.setValue(v);
		storage.store("PresetDragDropEnabled", v.name());
	}

	public void setDisplayScaling(DisplayScaling v) {
		Setup.get().localSettings.displayScaling.setValue(v);
		storage.store("UI Scaling", v.name());
	}

	public void setStripeBrightness(com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.StripeBrightness v) {
		Setup.get().localSettings.stripeBrightness.setValue(v);
		storage.store("StripeBrightness", v.name());
	}

	public void setBitmapCache(BooleanValues v) {
		Setup.get().localSettings.bitmapCache.setValue(v);
		storage.store("BitmapCache", v.name());
	}

	public void showDeveloperOptions(BooleanValues v) {
		boolean show = v == BooleanValues.on;

		if (show) {
			Window.alert("For developers only!");
		} else {
			Window.alert("Changes of the developer options will be discarded.");
			NonMaps.theMaps.getNonLinearWorld().getSettings().setToDefaults();
		}

		Setup.get().localSettings.showDeveloperOptions.setValue(v);
		storage.store("ShowDeveloperOptions", v.name());
	}
}
