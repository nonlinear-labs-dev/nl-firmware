package com.nonlinearlabs.client.dataModel.presetManager;

import java.util.HashMap;

public class PresetManagerModel {

	static PresetManagerModel thePresetManager = new PresetManagerModel();
	private BankMapDataModelEntity banks = new BankMapDataModelEntity(new HashMap<String, Bank>());

	static public PresetManagerModel get() {
		return thePresetManager;
	}

	public BankMapDataModelEntity getBanks() {
		return banks;
	}
}
