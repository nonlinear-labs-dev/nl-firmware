package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Pedal;

public class PedalSetting extends Setting {

	private Pedal pedal;

	public PedalSetting(Control parent, Pedal pedal, String name) {
		super(parent, name);
		this.pedal = pedal;
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new PedalSettings(this);
	}

	public Parameter getPedal() {
		return pedal;
	}

}
