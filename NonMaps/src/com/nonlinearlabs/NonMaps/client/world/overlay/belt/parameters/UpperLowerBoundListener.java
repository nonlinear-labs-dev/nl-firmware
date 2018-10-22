package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.Tracer;
import com.nonlinearlabs.NonMaps.client.tools.NLMath;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.value.QuantizedClippedValue;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters.BeltParameterLayout.Mode;

class UpperLowerBoundListener implements QuantizedClippedValue.ChangeListener {
	private final boolean upper;

	UpperLowerBoundListener(boolean upper) {
		this.upper = upper;
	}

	@Override
	public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newQuantizedValue) {
				
		BeltParameterLayout bpm = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getBelt().getParameterLayout();
		
		if(bpm != null) {
			if(upper && bpm.getMode() == Mode.mcLower)
				return;
			else if (!upper && bpm.getMode() == Mode.mcUpper)
				return;
		}
		
		Parameter p = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter modulatedParam = (ModulatableParameter) p;
			MacroControls s = modulatedParam.getModulationSource();
			if (s != MacroControls.NONE) {
				MacroControlParameter mc = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getMacroControls().getControl(s);

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
				newValue = modulatedParam.getValue().getQuantizedValue(newValue, true);

				
				if (modulatedParam.isBiPolar())
					newModAmount /= 2;
								
				modulatedParam.getModulationAmount().setRawValue(Initiator.INDIRECT_USER_ACTION, newModAmount);
				modulatedParam.getValue().setRawValue(Initiator.INDIRECT_USER_ACTION, newValue);

				if (initiator == Initiator.EXPLICIT_USER_ACTION)
					NonMaps.theMaps.getServerProxy().setModulationAmountAndValue(modulatedParam, newModAmount, newValue);
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