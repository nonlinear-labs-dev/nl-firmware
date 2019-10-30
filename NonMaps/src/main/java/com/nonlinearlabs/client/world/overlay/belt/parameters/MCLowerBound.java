package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.client.world.maps.parameters.value.QuantizedClippedValue;

public class MCLowerBound extends MCSomething {

	QuantizedClippedValue dummyValue;

	public MCLowerBound(Control parent) {
		super(parent);
		addChild(middle = new MCLowerBoundLabel(this));
	}

	@Override
	public MCLowerBoundLabel getMiddle() {
		return (MCLowerBoundLabel) super.getMiddle();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			createDummyValue(m);

			getParent().setMode(BeltParameterLayout.Mode.mcAmount);

			if (m.getModulationSource() != MacroControls.NONE)
				getParent().setValueChanger(dummyValue.startUserEdit(getPixRect().getWidth()));
		}

		return getParent();
	}

	private void createDummyValue(ModulatableParameter p) {
		dummyValue = new QuantizedClippedValue(new QuantizedClippedValue.ChangeListener() {

			@Override
			public void onQuantizedValueChanged(double oldQuantizedValue, double newLeft) {
				Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

				if (p instanceof ModulatableParameter) {
					ModulatableParameter modulatedParam = (ModulatableParameter) p;
					MacroControls s = modulatedParam.getModulationSource();
					if (s != MacroControls.NONE) {
						MacroControlParameter mc = getNonMaps().getNonLinearWorld().getParameterEditor()
								.getMacroControls().getControl(s);

						double modAmount = modulatedParam.getModulationAmount().getClippedValue();

						if (modulatedParam.isBiPolar())
							modAmount *= 2;

						double srcValue = mc.getValue().getClippedValue();
						double value = modulatedParam.getValue().getClippedValue();
						double modLeft = value - modAmount * srcValue;
						double modRight = modLeft + modAmount;

						double newModAmount = modRight - newLeft;
						double newValue = newLeft + newModAmount * srcValue;
						newValue = modulatedParam.getValue().clip(newValue);
						newValue = modulatedParam.getValue().getQuantizedValue(newValue, true);

						if (modulatedParam.isBiPolar())
							newModAmount /= 2;

						modulatedParam.getModulationAmount().setRawValue(newModAmount);
						modulatedParam.getValue().setRawValue(newValue);
						NonMaps.theMaps.getServerProxy().setModulationAmountAndValue(modulatedParam, newModAmount,
								newValue);
					}
				}
			}

			@Override
			public void onClippedValueChanged(double oldClippedValue, double newClippedValue) {
			}

			@Override
			public void onRawValueChanged(double oldRawValue, double newRawValue) {
			}
		});

		dummyValue.setCoarseDenominator(p.getValue().getCoarseDenominator());
		dummyValue.setFineDenominator(p.getValue().getFineDenominator());
		dummyValue.setBipolar(p.getValue().isBipolar());
		dummyValue.setRawValue(getMiddle().calcBound());
	}

	@Override
	public Control doubleClick() {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			createDummyValue(m);
			dummyValue.setToDefault(Parameter.);
		}

		return this;
	}

	@Override
	protected void inc(ModulatableParameter m, boolean fine) {
		createDummyValue(m);
		dummyValue.inc(fine);
	}

	@Override
	protected void dec(ModulatableParameter m, boolean fine) {
		createDummyValue(m);
		dummyValue.dec(fine);
	}
}
