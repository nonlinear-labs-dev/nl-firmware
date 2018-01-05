package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class SmallParameterName extends LabelSmall {
	public SmallParameterName(MapsLayout parent, Name name) {
		super(parent, name);
	}

	@Override
	protected double getBasicHeight() {
		return 20;
	}

	@Override
	protected double getBasicWidth() {
		return 80;
	}

	protected String getDisplayText() {
		Name n = getName();
		if (n != null)
			return getName().getShortName();

		return super.getDisplayText();
	}
}