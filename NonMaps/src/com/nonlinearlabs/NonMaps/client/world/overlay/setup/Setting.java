package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class Setting extends OverlayLayout {

	private SetupLabel label;
	private OverlayControl control;

	public Setting(Control parent, final String label) {
		super(parent);
		addChild(this.label = new SetupLabel(this, label));
	}

	@Override
	public void init() {
		super.init();
		addChild(this.control = createSettingsControl());
	}

	protected abstract OverlayControl createSettingsControl();

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double lineHeight = Millimeter.toPixels(10);
		label.doLayout(0, 0, 3 * w / 7, lineHeight);
		control.doLayout(label.getRelativePosition().getWidth(), 0, 4 * w / 7, lineHeight);
	}

	public void setLabelText(String txt) {
		label.setText(txt);
	}

	public OverlayControl getSettingsControl() {
		return control;
	}

}