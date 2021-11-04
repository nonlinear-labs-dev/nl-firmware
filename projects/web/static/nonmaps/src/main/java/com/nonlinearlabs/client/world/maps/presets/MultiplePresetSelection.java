package com.nonlinearlabs.client.world.maps.presets;

import java.util.ArrayList;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.client.world.maps.presets.bank.preset.Preset;

public class MultiplePresetSelection {
	protected ArrayList<String> selectedPresets = new ArrayList<String>();

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
		NonMaps.get().getServerProxy().deletePresets(getCSV(), false);
	}

	protected PresetManager getPresetManager() {
		return NonMaps.get().getNonLinearWorld().getPresetManager();
	}

	protected String getLastUuid() {
		try {
			return selectedPresets.get(selectedPresets.size() - 1);
		} catch (Exception e) {
			return null;
		}
	}

	public void handleDownKey() {
		Preset curr = getPresetManager().findPreset(getLastUuid());
		if (curr != null) {
			Preset next = getPresetManager().findPreset(getLastUuid()).getParent().getPresetList()
					.getNext(getLastUuid());
			if (next != null) {
				if (contains(next)) {
					remove(curr);
				} else {
					add(next);
				}
			}
		}

	}

	public void handleUpKey() {
		Preset curr = getPresetManager().findPreset(getLastUuid());
		if (curr != null) {
			Preset prev = getPresetManager().findPreset(getLastUuid()).getParent().getPresetList()
					.getPrev(getLastUuid());
			if (prev != null) {
				if (contains(prev)) {
					remove(curr);
				} else {
					add(prev);
				}
			}
		}
	}

	public boolean selectionContainsSolePresets() {
		for (String s : getSelectedPresets()) {
			Preset p = getPresetManager().findPreset(s);
			if (p != null) {
				if (allPresetsOfBankInMulSelection(p.getParent()))
					return true;
			}
		}
		return false;
	}

	private boolean allPresetsOfBankInMulSelection(Bank b) {
		for (MapsControl p : b.getPresetList().getChildren()) {
			if (p instanceof Preset) {
				if (!selectedPresets.contains(((Preset) p).getUUID()))
					return false;
			}
		}
		return true;
	}
}
