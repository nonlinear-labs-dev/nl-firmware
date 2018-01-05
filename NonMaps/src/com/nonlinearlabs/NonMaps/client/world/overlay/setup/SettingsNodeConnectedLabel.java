package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.Control;

public abstract class SettingsNodeConnectedLabel extends SettingsControl {

	private SetupLabel label;

	public SettingsNodeConnectedLabel(Control parent) {
		super(parent);
		addChild(label = new SetupLabel(this, "---"));
	}

	@Override
	public void update(Node settingsNode, Node deviceInfo) {
		String str = getLabelText(settingsNode, deviceInfo);

		if (str != null && !str.isEmpty())
			label.setText(str);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		label.doLayout(0, 0, w, h);
	}

	public abstract String getLabelText(Node settingsNode, Node deviceInfo);

}
