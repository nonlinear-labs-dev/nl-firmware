package com.nonlinearlabs.NonMaps.client.presenters;

import java.util.ArrayList;

public class PresetManager {
	public static class Bank {

		public static class Preset {
			public String uuid;
			public String name;
		}

		public Bank(com.nonlinearlabs.NonMaps.client.dataModel.presetManager.Bank bank) {
			bank.name.onChange(n -> {
				name = n;
				return true;
			});
		}

		public String name;
		public String uuid;
	}

	public ArrayList<Bank> banks = new ArrayList<Bank>();
}
