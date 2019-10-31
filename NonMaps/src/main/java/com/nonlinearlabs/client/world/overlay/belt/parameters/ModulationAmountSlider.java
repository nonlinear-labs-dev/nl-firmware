package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.contextStates.ClipContext;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
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
			ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;
			if (p.modulation.isModulated) 
				drawSlider(ctx, p);
		}
	}

	protected void drawSlider(Context2d ctx, ParameterPresenter p) {
		// TODO
		Rect r = getPixRect().copy();
		boolean isMCHighlight = isHighlit();
		double width = r.getWidth();
		double targetX = 0;
	
		//if (!upper)
		//	r.moveBy(0, oldHeight - r.getHeight());

		RGB darker = isMCHighlight ? new Gray(179) : new Gray(102);
		RGB lighter = isMCHighlight ? new Gray(230) : new Gray(128);

	//	drawTopAndBottomContour(ctx, leftRect, fillColor);
//		fillColor = modLeft < modRight ? lighter : darker;
//		rightRect.fill(ctx, fillColor);

//		drawTopAndBottomContour(ctx, rightRect, fillColor);
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
