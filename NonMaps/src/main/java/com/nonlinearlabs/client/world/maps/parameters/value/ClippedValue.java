package com.nonlinearlabs.client.world.maps.parameters.value;

import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;
import com.nonlinearlabs.client.tools.NLMath;
import com.nonlinearlabs.client.world.maps.parameters.Parameter.Initiator;

class ClippedValue extends Value {

	public double getClippedValue() {
		return clip(getRawValue());
	}

	public double clip(double v) {
		return NLMath.clamp(v, getLowerBorder(), getUpperBorder());
	}

	public double getUpperBorder() {
		return 1.0;
	}

	public double getLowerBorder() {
		return isBipolar() ? -1.0 : 0.0;
	}

	@Override
	protected void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
		double oldClippedValue = clip(oldRawValue);
		double newClippedValue = clip(newRawValue);

		if (oldClippedValue != newClippedValue)
			onClippedValueChanged(initiator, oldClippedValue, newClippedValue);

		super.onRawValueChanged(initiator, oldRawValue, newRawValue);
	}

	protected void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
	}

	public void update(ValueDataModelEntity e) {
		super.update(e);

	}

}
