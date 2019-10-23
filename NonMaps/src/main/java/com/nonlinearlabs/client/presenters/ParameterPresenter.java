package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.Tracer;

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
	public boolean isBoolean = false;
	public String shortName = "---";

	public long hash = 0;

	public static class Modulation {
		static public String[] sourceOptions = { "", "Ⓐ", "Ⓑ", "Ⓒ", "Ⓓ" };
		static int selectedSourceOption = 0;
		public Range modulationRange = null;
	}

	public void getHash(Checksum crc) {
		crc.eat(hash);

		if (shortName.contains("Offset +"))
			Tracer.log("Parameter " + shortName + " added its hash of " + hash + " to checksum: " + crc.getHash());
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

		long newHash = c.getHash();

		if (hash != newHash) {
			hash = newHash;
			if (shortName.contains("Offset +"))
				Tracer.log("Parameter " + shortName + " has new hash: " + hash);
			return true;
		}

		return false;
	}
}
