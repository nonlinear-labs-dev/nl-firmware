package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelLarge;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class ValueDisplay extends Label {

	public ValueDisplay(BeltParameterLayout parent) {
		super(parent);
		super.setFontHeightInMM(6);
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public String getDrawText(Context2d ctx) {
		String[] str = getParent().getDecoratedValue(true);

		for (String s : str)
			if (ctx.measureText(s).getWidth() < getPixRect().getWidth() * 0.8)
				return s;

		return "";
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (getPixRect().getWidth() >= Millimeter.toPixels(20))
			super.draw(ctx, invalidationMask);
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		if (amount > 0)
			getParent().getValue().inc(Initiator.EXPLICIT_USER_ACTION, fine);
		else if (amount < 0)
			getParent().getValue().dec(Initiator.EXPLICIT_USER_ACTION, fine);

		return this;
	}
}
