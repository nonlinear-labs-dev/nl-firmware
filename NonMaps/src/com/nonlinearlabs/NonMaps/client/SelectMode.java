package com.nonlinearlabs.NonMaps.client;

import com.nonlinearlabs.NonMaps.client.world.IPreset;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;

public class SelectMode {
	private PresetManager m_parent;
	private IPreset selectedPreset;
	private IPreset initialSelectedPreset;
	
	public SelectMode(PresetManager parent) {
		m_parent = parent;
		initialSelectedPreset = m_parent.getSelectedPreset();
		selectedPreset = initialSelectedPreset;
	}
	
	public IPreset getInitalSelectedPreset() {
		return initialSelectedPreset;
	}
	
	public void setSelectedPreset(IPreset p) {
		selectedPreset = p;
	}
	
	public IPreset getSelectedPreset() {
		return selectedPreset;
	}

	public void selecteNextBank() {
		
	}
	
	public void selectePreviousBank() {
		
	}
}

