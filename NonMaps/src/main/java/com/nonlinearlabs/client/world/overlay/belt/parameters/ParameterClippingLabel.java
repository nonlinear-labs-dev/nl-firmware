package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.tools.NLMath;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Range;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.client.world.maps.parameters.value.ModulationAmount;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

public class ParameterClippingLabel extends Label {
	BeltParameterLayout.Mode mMode;

	ParameterClippingLabel(OverlayLayout parent, BeltParameterLayout.Mode mode) {
		super(parent);
		setFontHeightInMM(6);
		mMode = mode;
	}

	public boolean isClipping() {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();
		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			MacroControls s = m.getModulationSource();

			if (s != MacroControls.NONE) {
				ModulationAmount amount = m.getModulationAmount();
				MacroControlParameter mc = getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls()
						.getControl(s);
				double modAmount = amount.getClippedValue();

				if (m.isBiPolar())
					modAmount *= 2;

				double srcValue = mc.getValue().getClippedValue();
				double value = m.getValue().getClippedValue();

				Range bounds = new Range(m.isBiPolar() ? -1.0 : 0, 1.0);
				double left = (value - modAmount * srcValue);
				double right = left + modAmount;
				Range mod = new Range(left, right);
				Range modNormalized = new Range(mod.getLeft(), mod.getRight());
				modNormalized.normalize();

				double r = NLMath.quantize(modNormalized.getRight(), 1000);
				double l = NLMath.quantize(modNormalized.getLeft(), 1000);

				if (mMode == Mode.mcLower)
					return bounds.outOfRange(l);
				else if (mMode == Mode.mcUpper)
					return bounds.outOfRange(r);
			}
		}

		return false;
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return isClipping() ? "!" : "";
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (isClipping()) {
			getPixRect().fill(ctx, new RGB(51, 51, 51));
			super.draw(ctx, invalidationMask);
		}
	}

}
