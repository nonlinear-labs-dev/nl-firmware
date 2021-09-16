package com.nonlinearlabs.client.world.maps.parameters.MonoAndUnison;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;
import com.nonlinearlabs.client.world.maps.parameters.Spacer;

class UnisonControls extends ParameterGroup {

	UnisonControls(MapsLayout parent) {
		super(parent, "Unison");
		addChild(new Spacer(this, 1, 1));
		addChild(new LabelModuleHeader(this) {
			@Override
			public RGB getColorFont() {
				return RGB.lighterGray();
			}

			@Override
			public void drawBackground(Context2d ctx, Rect pixRect) {
				pixRect.drawRoundedRect(ctx, Rect.ROUNDING_TOP, toXPixels(6), toXPixels(2), new Gray(87), null);
			}

			@Override
			public double getBottomMargin() {
				return -2;
			}

			@Override
			protected double getFontHeight() {
				return 13;
			}

			@Override
			protected double getBasicHeight() {
				return 22;
			}
		});
		addChild(new UnisonColumn(this));
	}

	@Override
	public void drawBorder(Context2d ctx) {
		return;
	}

	@Override
	public Control doubleClick(Position pos) {
		return this;
	}

	@Override
	public double getXMargin() {
		return 1;
	}

	@Override
	public double getYMargin() {
		return 1;
	}
}
