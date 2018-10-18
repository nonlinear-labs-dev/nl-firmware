package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.Map;

import com.nonlinearlabs.NonMaps.client.dataModel.DataModelEntity;

public class BankMapDataModelEntity extends DataModelEntity<Map<String, Bank>> {

	public BankMapDataModelEntity(Map<String, Bank> def) {
		super(def);
	}

	@Override
	public void fromString(String str) {
		assert (false);
	}

}
