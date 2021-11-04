package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.Label;

public class MCUpperBoundLabel extends Label {

	public MCUpperBoundLabel(MCUpperBound parent) {
		super(parent);
	}

	@Override
	public MCUpperBound getParent() {
		return (MCUpperBound) super.getParent();
	}

	@Override
	public String getDrawText(Context2d ctx) {
		ParameterPresenter p = EditBufferPresenterProvider.getPresenter().selectedParameter;

		if (p.modulation.isModulated)
			return chooseFittingString(ctx, p.modulation.amountUpperBoundDisplayValues);

		String str[] = { "MC Upper Bound", "MC Upper", "Up" };
		return chooseFittingString(ctx, str);

	}

	@Override
	protected RGB getColorFontForSplit(int i) {
		if (i == 0)
			return super.getColorFontForSplit(i).darker(80);

		return super.getColorFontForSplit(i);
	}
	
	protected Rect getTextRect() {
		Rect r = super.getTextRect().copy();
		r.reduceWidthBy(Millimeter.toPixels(5));
		return r;
	}
}
