package com.nonlinearlabs.client.dataModel.presetManager;

import java.util.ArrayList;

import com.nonlinearlabs.client.dataModel.StringDataModelEntity;

public class Bank extends Doomable {
	private PresetListDataModelEntity presets = new PresetListDataModelEntity(new ArrayList<Preset>());

	public StringDataModelEntity name = new StringDataModelEntity();
	public StringDataModelEntity uuid = new StringDataModelEntity();

	public PresetListDataModelEntity getPresets() {
		return presets;
	}

}