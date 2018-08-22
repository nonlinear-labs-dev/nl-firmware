package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.ArrayList;

public class PresetManager {

	public class Bank {
		public class Preset {
		}

		ArrayList<Preset> presets = new ArrayList<Preset>();

		ArrayList<Preset> getPresets() {
			return presets;
		}
	}

	static PresetManager thePresetManager = new PresetManager();
	ArrayList<Bank> banks = new ArrayList<Bank>();

	private PresetManager() {
	}

	static public PresetManager get() {
		return thePresetManager;
	}

	ArrayList<Bank> getBanks() {
		return banks;
	}
}
