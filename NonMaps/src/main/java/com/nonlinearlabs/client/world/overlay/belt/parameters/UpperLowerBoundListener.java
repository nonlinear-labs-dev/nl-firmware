package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.client.world.maps.parameters.value.QuantizedClippedValue;

class UpperLowerBoundListener implements QuantizedClippedValue.ChangeListener {
	private final boolean upper;

	UpperLowerBoundListener(boolean upper) {
		this.upper = upper;
	}

	@Override
	public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newQuantizedValue) {
		Parameter p = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter modulatedParam = (ModulatableParameter) p;
			MacroControls s = modulatedParam.getModulationSource();
			if (s != MacroControls.NONE) {
				MacroControlParameter mc = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getMacroControls()
						.getControl(s);

				double modAmount = modulatedParam.getModulationAmount().getClippedValue();

				if (modulatedParam.isBiPolar())
					modAmount *= 2;

				double srcValue = mc.getValue().getClippedValue();
				double value = modulatedParam.getValue().getClippedValue();
				double modLeft = value - modAmount * srcValue;

				double newModAmount = 0;
				double newValue = 0;

				if (upper) {
					newModAmount = newQuantizedValue - modLeft;
					newValue = modLeft + newModAmount * srcValue;

				} else {
					double modRight = modLeft + modAmount;
					newModAmount = modRight - newQuantizedValue;
					newValue = newQuantizedValue + newModAmount * srcValue;
				}

				newValue = modulatedParam.getValue().clip(newValue);

				if (modulatedParam.isBiPolar())
					newModAmount /= 2;

				if (initiator == Initiator.EXPLICIT_USER_ACTION) {
					modulatedParam.getModulationAmount().setRawValue(Initiator.INDIRECT_USER_ACTION, newModAmount);
					modulatedParam.getValue().setRawValue(Initiator.INDIRECT_USER_ACTION, newValue);

					NonMaps.theMaps.getServerProxy().setModulationAmountAndValue(modulatedParam, newModAmount,
							newValue);
				}
			}
		}
	}

	@Override
	public void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
	}

	@Override
	public void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
	}
}