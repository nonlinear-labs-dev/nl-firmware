package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;

public class MCAmountEditLabel extends Label {

	public MCAmountEditLabel(MCSomething parent) {
		super(parent);
	}

	@Override
	public MCAmountEdit getParent() {
		return (MCAmountEdit) super.getParent();
	}

	@Override
	public String getDrawText(Context2d ctx) {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			MacroControls s = m.getModulationSource();
			if (s != MacroControls.NONE) {
				String decorated = m.getModulationAmount().getDecoratedValue(true);
				String unDecorated = m.getModulationAmount().getDecoratedValue(false);

				String str[] = { "MC Amount: \t" + decorated, "MC Amount: \t" + unDecorated, "MC Amt: \t" + unDecorated,
						"Amt: \t" + unDecorated, unDecorated };

				return chooseFittingString(ctx, str);

			}
		}

		String str[] = { "MC Amount", "MC Amt", "Amt" };
		return chooseFittingString(ctx, str);
	}

	@Override
	protected RGB getColorFontForSplit(int i) {
		if (i == 0)
			return super.getColorFontForSplit(i).darker(80);

		return super.getColorFontForSplit(i);
	}

	private native String stringize(boolean withUnit, double value) /*-{
																	var intermediate = Number(value).toPrecision(3);
																	var ret = parseFloat(intermediate);
																	
																	if (withUnit)
																	return ret + "%";
																	
																	return ret + "";
																	}-*/;

	protected Rect getTextRect() {
		Rect r = super.getTextRect().copy();
		r.reduceWidthBy(Millimeter.toPixels(5));
		return r;
	}
}
