package com.nonlinearlabs.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.Name;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;

public class ParameterName extends LabelLarge {

	public ParameterName(ModulatableParameter p) {
		super(p);
	}

	public ParameterName(ModulatableParameter p, String name) {
		super(p, name);
	}

	@Override
	public ModulatableParameter getParent() {
		return (ModulatableParameter) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(4), 0, getNameBackgroundColor(), null);
		super.draw(ctx, invalidationMask);
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	private RGB getNameBackgroundColor() {
		return getParent().isSelected() ? getColorObjectContourSelected() : new Gray(87);
	}

	private int getBackgroundRoundings() {
		return getParent().getBackgroundRoundings() & Rect.ROUNDING_TOP;
	}

	@Override
	public RGB getColorFont() {
		return getParent().isSelected() ? getColorSliderBackground() : super.getColorFont();
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
