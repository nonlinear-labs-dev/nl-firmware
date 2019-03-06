package com.nonlinearlabs.NonMaps.client.presenters;

import com.nonlinearlabs.NonMaps.client.Checksum;

public class ParameterPresenter {
	public class Range {
		public double left = 0;
		public double value = 0;
		public double right = 0;
	}

	public String displayValue = "---";
	public Range valueRange = new Range();
	public boolean bipolar = false;
	public boolean selected = false;
	public boolean locked = false;
	public boolean isChanged = false;
	public boolean isBoolean = false;
	public boolean isModulateable = false;
	public boolean isModulationChanged = false;
	
	public String shortName = "---";

	public long hash = 0;

	public static class Modulation {
		static public String[] sourceOptions = { "", "Ⓐ", "Ⓑ", "Ⓒ", "Ⓓ" };
		static int selectedSourceOption = 0;
		public Range modulationRange = null;
	}

	public void getHash(Checksum crc) {
		crc.eat(hash);
	}

	public boolean updateHash() {
		Checksum c = new Checksum();

		c.eat(valueRange.left);
		c.eat(valueRange.right);
		c.eat(valueRange.value);
		c.eat(bipolar);
		c.eat(selected);
		c.eat(locked);
		c.eat(isBoolean);
		c.eat(shortName);
		c.eat(displayValue);
		c.eat(isModulateable);
		c.eat(isModulationChanged);
		c.eat(isChanged);
		
		long newHash = c.getHash();

		if (hash != newHash) {
			hash = newHash;
			return true;
		}

		return false;
	}
}
