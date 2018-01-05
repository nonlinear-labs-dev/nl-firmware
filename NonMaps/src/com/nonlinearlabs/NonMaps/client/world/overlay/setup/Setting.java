package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class Setting extends OverlayLayout {

	private SetupLabel label;
	private SettingsControl control;

	public Setting(Control parent, final String label) {
		super(parent);

		addChild(this.label = new SetupLabel(this, label));
		addChild(this.control = createSettingsControl());
	}

	protected abstract SettingsControl createSettingsControl();

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double lineHeight = getButtonDimension();
		label.doLayout(0, 0, 3 * w / 7, lineHeight);
		control.doLayout(label.getRelativePosition().getWidth(), 0, 4 * w / 7, lineHeight);
	}

	public void update(Node settingsNode, Node deviceInfo) {
		control.update(settingsNode, deviceInfo);
	}

	public void setLabelText(String txt) {
		label.setText(txt);
	}

	public SettingsControl getSettingsControl() {
		return control;
	}

}