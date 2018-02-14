package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.value.ModulationAmount;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;

public class MCLowerBoundLabel extends Label {

	public MCLowerBoundLabel(MCLowerBound parent) {
		super(parent);
	}

	@Override
	public MCLowerBound getParent() {
		return (MCLowerBound) super.getParent();
	}

	@Override
	public String getDrawText(Context2d ctx) {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			MacroControls s = m.getModulationSource();
			if (s != MacroControls.NONE) {
				double v = calcBound();
				String withUnit = p.getDecoratedValue(true, v);
				String withoutUnit = p.getDecoratedValue(false, v);

				String str[] = { "MC Lower Bound: \t" + withUnit, "MC Lower: \t" + withUnit, "MC Low: \t" + withoutUnit,
						"Lo: \t" + withoutUnit, withoutUnit };
				return chooseFittingString(ctx, str);

			}
		}

		String str[] = { "MC Lower Bound", "MC Low", "Lo" };
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

	public double calcBound() {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			MacroControls s = m.getModulationSource();
			if (s != MacroControls.NONE) {
				ModulationAmount amount = m.getModulationAmount();
				MacroControlParameter mc = getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls().getControl(s);

				double modAmount = amount.getClippedValue();

				if (m.isBiPolar())
					modAmount *= 2;

				double srcValue = mc.getValue().getClippedValue();
				double value = m.getValue().getClippedValue();
				double modLeft = value - modAmount * srcValue;
				return m.getValue().clip(modLeft);
			}
		}
		return 0;
	}
}
