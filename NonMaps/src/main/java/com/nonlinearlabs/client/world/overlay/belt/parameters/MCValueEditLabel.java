package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.overlay.Label;

public class MCValueEditLabel extends Label {

	public MCValueEditLabel(MCValueEdit parent) {
		super(parent);
	}

	@Override
	public MCValueEdit getParent() {
		return (MCValueEdit) super.getParent();
	}

	@Override
	public String getDrawText(Context2d ctx) {
		Parameter q = getParent().getMacroControl();

		if (q != null) {
			String decorated = q.getDecoratedValue(true);
			String unDecorated = q.getDecoratedValue(false);

			String str[] = { "MC Position: \t" + decorated, "MC Position: \t" + unDecorated, "MC Pos: \t" + unDecorated,
					"Pos: \t" + unDecorated, unDecorated };

			return chooseFittingString(ctx, str);
		}

		String str[] = { "MC Position", "MC Pos", "Pos" };
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
