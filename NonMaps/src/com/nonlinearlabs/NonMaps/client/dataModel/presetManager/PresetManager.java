package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.HashMap;

public class PresetManager {

	static PresetManager thePresetManager = new PresetManager();
	private BankMapDataModelEntity banks = new BankMapDataModelEntity(new HashMap<String, Bank>());

	static public PresetManager get() {
		return thePresetManager;
	}

	public BankMapDataModelEntity getBanks() {
		return banks;
	}
}
