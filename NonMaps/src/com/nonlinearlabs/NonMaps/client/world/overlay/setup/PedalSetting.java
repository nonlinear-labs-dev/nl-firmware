package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources.Pedal;

public class PedalSetting extends Setting {

	private Pedal pedal;

	public PedalSetting(Control parent, Pedal pedal) {
		super(parent, "");
		this.pedal = pedal;
	}

	@Override
	protected SettingsControl createSettingsControl() {
		return new PedalSettings(this);
	}

	public Parameter getPedal() {
		return pedal;
	}

	@Override
	public void update(Node settingsNode, Node deviceInfo) {
		super.update(settingsNode, deviceInfo);
		setLabelText(pedal.getName().getLongName());
	}
}
