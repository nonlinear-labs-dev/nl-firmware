package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.Tracer;
import com.nonlinearlabs.client.dataModel.setup.Setup;

public class ParameterPresenter {
	public static class Range {
		public double left = 0;
		public double right = 0;
	}

	public int id = 0;
	public String[] displayValues = new String[] { "" };
	public double controlPosition = 0;
	public boolean bipolar = false;
	public boolean selected = false;
	public boolean locked = false;
	public boolean isBoolean = false;
	public boolean changed = false;
	public String shortName = "---";
	public String longName = "---";
	public String fullNameWithGroup = "";

	public long hash = 0;

	public static class Modulation {
		public Range modulationRange = new Range();
		public String modulationSourceLabel = "";
		public boolean isModulated = false;
		public boolean isModulateable = false;
		public String[] amountDisplayValues = new String[] { "" };
		public String[] amountLowerBoundDisplayValues = new String[] { "" };
		public String[] amountUpperBoundDisplayValues = new String[] { "" };
		public String[] macroControlDisplayValues = new String[] { "" };
	}

	public Modulation modulation = new Modulation();
	public boolean highlightChanged = false;
	public String userGivenName = "";
	public boolean showContextMenu = false;

	public void getHash(Checksum crc) {
		crc.eat(hash);

		if (shortName.contains("Offset +"))
			Tracer.log("Parameter " + shortName + " added its hash of " + hash + " to checksum: " + crc.getHash());
	}

	public boolean updateHash() {
		Checksum c = new Checksum();

		c.eat(controlPosition);
		c.eat(bipolar);
		c.eat(selected);
		c.eat(locked);
		c.eat(isBoolean);
		c.eat(shortName);
		c.eat(displayValues);
		c.eat(changed);

		c.eat(Setup.get().systemSettings.highlightChangedParameters.getValue().toString());
		c.eat(Setup.get().systemSettings.forceHighlightChangedParameters.getValue().toString());

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
