package com.nonlinearlabs.NonMaps.client.world.maps.presets;

import java.util.ArrayList;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

public class MultiplePresetSelection {
	private ArrayList<String> selectedPresets = new ArrayList<String>();

	public MultiplePresetSelection(Preset p) {
		add(p);
	}

	public MultiplePresetSelection() {

	}

	public ArrayList<String> getSelectedPresets() {
		return selectedPresets;
	}

	public void add(Preset p) {
		if (!contains(p)) {
			selectedPresets.add(p.getUUID());
		}
	}

	public boolean contains(Preset p) {
		return selectedPresets.contains(p.getUUID());
	}

	public void remove(Preset p) {
		if (contains(p)) {
			selectedPresets.remove(p.getUUID());
		}
	}

	public void toggle(Preset p) {
		if (contains(p))
			selectedPresets.remove(p.getUUID());
		else
			selectedPresets.add(p.getUUID());
	}

	public int getNumSelectedPresets() {
		return selectedPresets.size();
	}

	public boolean isEmpty() {
		return selectedPresets.isEmpty();
	}

	public void clear() {
		selectedPresets.clear();
	}

	public String getCSV() {
		String ret = "";
		for (String a : selectedPresets) {
			if (!ret.isEmpty())
				ret += ",";
			ret += a;
		}
		return ret;
	}

	public void deletePresets() {
		NonMaps.get().getServerProxy().deletePresets(getCSV());
	}
}