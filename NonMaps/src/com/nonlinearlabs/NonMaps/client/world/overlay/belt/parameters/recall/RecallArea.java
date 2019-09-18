package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.recall;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout;

public abstract class RecallArea extends OverlayLayout {

	protected RecallValue value;
	protected RecallButton button;

	protected RecallArea(BeltParameterLayout parent) {
		super(parent);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		button.setActive(isChanged());
		value.setActive(isChanged());

		final double padding = Millimeter.toPixels(6);
		final double textWidth = value.calculateTextWidth() + padding;
		final double buttonX = w / 2 - textWidth / 2;
		final double buttonY = h / 4;
		final double half = (h / 2);

		value.doLayout(0, buttonY + 1, w, half);
		button.doLayout(buttonX, buttonY, textWidth, half);
	}

	abstract public void setVisibleForMode(BeltParameterLayout.Mode mode);

	abstract public boolean isChanged();
}
