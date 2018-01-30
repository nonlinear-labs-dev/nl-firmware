package com.nonlinearlabs.NonMaps.client;

import com.nonlinearlabs.NonMaps.client.world.IPreset;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;

public class StoreMode {
	private PresetManager m_parent;
	private IPreset selectedPreset;
	
	public StoreMode(PresetManager parent) {
		m_parent = parent;
		selectedPreset = m_parent.getSelectedPreset();
	}
	
	public void setSelectedPreset(IPreset p) {
		selectedPreset = p;
	}
	
	public IPreset getSelectedPreset() {
		return selectedPreset;
	}
}
