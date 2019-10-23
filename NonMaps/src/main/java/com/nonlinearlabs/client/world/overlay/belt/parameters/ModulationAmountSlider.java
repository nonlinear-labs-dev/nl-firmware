package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.contextStates.ClipContext;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.client.world.overlay.OverlayControl;

public class ModulationAmountSlider extends OverlayControl {

	private boolean upper;

	public ModulationAmountSlider(Sliders parent, boolean upper) {
		super(parent);
		this.upper = upper;
	}

	@Override
	public Sliders getParent() {
		return (Sliders) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		try (ClipContext c = new ClipContext(ctx, this)) {
			Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();
			if (p instanceof ModulatableParameter) {
				ModulatableParameter modulatedParameter = (ModulatableParameter) p;
				MacroControls modSource = modulatedParameter.getModulationSource();

				if (modSource != MacroControls.NONE)
					drawSlider(ctx, modulatedParameter, modSource);
			}
		}
	}

	protected void drawSlider(Context2d ctx, ModulatableParameter modulatedParameter, MacroControls modSource) {
		Parameter modulator = getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls()
				.getControl(modSource);

		boolean isBiPolar = modulatedParameter.isBiPolar();
		double modulationAmount = modulatedParameter.getModulationAmount().getQuantizedClipped();

		if (isBiPolar)
			modulationAmount *= 2;

		double targetValue = modulatedParameter.getValue().getRawValue();
		double macroControlValue = modulator.getValue().getClippedValue();
		double modLeft = targetValue - modulationAmount * macroControlValue;
		double modRight = targetValue + modulationAmount * (1 - macroControlValue);

		Rect r = getPixRect().copy();
		boolean isMCHighlight = isHighlit();

		double width = r.getWidth();
		double targetX = 0;

		if (isBiPolar) {
			double x = r.getCenterPoint().getX();
			r.setLeft(x + width / 2 * modLeft);
			r.setRight(x + width / 2 * modRight);
			targetX = x + width / 2 * targetValue;
		} else {
			double left = r.getLeft();
			r.setLeft(left + width * modLeft);
			r.setRight(left + width * modRight);
			targetX = left + width * targetValue;
		}

		r = normalizeRect(r);

		double oldHeight = r.getHeight();
		r.setHeight(r.getHeight() * 0.75);

		if (!upper)
			r.moveBy(0, oldHeight - r.getHeight());

		RGB darker = isMCHighlight ? new Gray(179) : new Gray(102);
		RGB lighter = isMCHighlight ? new Gray(230) : new Gray(128);

		Rect leftRect = r.copy();
		leftRect.setRight(targetX);
		RGB fillColor = modLeft < modRight ? darker : lighter;
		leftRect.fill(ctx, fillColor);
		drawTopAndBottomContour(ctx, leftRect, fillColor);

		Rect rightRect = r.copy();
		double oldWidth = rightRect.getRight() - targetX;
		rightRect.setLeft(targetX);
		rightRect.setWidth(oldWidth);

		fillColor = modLeft < modRight ? lighter : darker;
		rightRect.fill(ctx, fillColor);

		drawTopAndBottomContour(ctx, rightRect, fillColor);
	}

	protected void drawTopAndBottomContour(Context2d ctx, Rect leftRect, RGB fillColor) {
		ctx.setLineWidth(2);
		ctx.setStrokeStyle(fillColor.darker(40).toString());

		ctx.beginPath();
		ctx.moveTo(leftRect.getLeft(), leftRect.getTop());
		ctx.lineTo(leftRect.getRight(), leftRect.getTop());
		ctx.stroke();

		ctx.beginPath();
		ctx.moveTo(leftRect.getLeft(), leftRect.getBottom());
		ctx.lineTo(leftRect.getRight(), leftRect.getBottom());
		ctx.stroke();
	}

	protected Rect normalizeRect(Rect r) {
		r.normalize();
		r = r.getIntersection(getPixRect());
		return r;
	}

	protected boolean isHighlit() {
		return getParent().getParent().isOneOf(BeltParameterLayout.Mode.mcAmount, BeltParameterLayout.Mode.mcValue,
				BeltParameterLayout.Mode.mcLower, BeltParameterLayout.Mode.mcUpper);
	}
}
