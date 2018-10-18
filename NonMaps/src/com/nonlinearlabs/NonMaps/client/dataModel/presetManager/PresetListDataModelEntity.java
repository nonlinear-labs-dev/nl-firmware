package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.ArrayList;

import com.nonlinearlabs.NonMaps.client.dataModel.DataModelEntity;

public class PresetListDataModelEntity extends DataModelEntity<ArrayList<Preset>> {

	public PresetListDataModelEntity(ArrayList<Preset> def) {
		super(def);
	}

	@Override
	public void fromString(String str) {
		assert (false);
	}

}
