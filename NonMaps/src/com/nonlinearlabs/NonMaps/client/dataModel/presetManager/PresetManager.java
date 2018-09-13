package com.nonlinearlabs.NonMaps.client.dataModel.presetManager;

import java.util.HashMap;
import java.util.Map;

public class PresetManager {

	static PresetManager thePresetManager = new PresetManager();
	private Map<String, Bank> banks = new HashMap<String, Bank>();

	static public PresetManager get() {
		return thePresetManager;
	}

	Map<String, Bank> getBanks() {
		return banks;
	}
}
