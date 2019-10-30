package com.nonlinearlabs.client.useCases;

import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.setup.Setup.BooleanValues;

public class IncrementalChanger {
	private final BasicParameterModel parameter;
	private double lastQuantizedValue = 0;
	private double pendingAmount = 0;
	private double pixPerRange = 0;

	IncrementalChanger(BasicParameterModel parameter, double pixPerRange) {
		this.parameter = parameter;
		this.lastQuantizedValue = parameter.value.value.getValue();
		this.pixPerRange = pixPerRange;
	}

	public ValueDataModelEntity getValue() {
		return this.parameter.value;
	}

	public double getQuantizedValue(double v, boolean fine) {
		double steps = fine ? parameter.value.metaData.fineDenominator.getValue()
				: parameter.value.metaData.coarseDenominator.getValue();
		v *= steps;
		v = Math.round(v);
		return v / steps;
	}

	public double clip(double v) {
		return Math.min(getUpperBorder(), Math.max(v, getLowerBorder()));
	}

	public double getUpperBorder() {
		return 1.0;
	}

	public double getLowerBorder() {
		return (parameter.value.metaData.bipolar.getValue() == BooleanValues.on) ? -1.0 : 0.0;
	}

	public void changeBy(boolean fine, double amount) {

		amount /= pixPerRange;

		if (parameter.value.metaData.bipolar.getValue() == BooleanValues.on)
			amount *= 2;

		if (fine)
			amount = amount * parameter.value.metaData.coarseDenominator.getValue()
					/ parameter.value.metaData.fineDenominator.getValue();

		pendingAmount += amount;

		double newVal = getQuantizedValue(lastQuantizedValue + pendingAmount, fine);
		newVal = clip(newVal);

		if (newVal != lastQuantizedValue) {

			if (parameter.value.metaData.isBoolean.getValue() == BooleanValues.on) {
				if (newVal > lastQuantizedValue)
					newVal = 1.0;
				else if (newVal < lastQuantizedValue)
					newVal = 0.0;
			}

			EditBufferUseCases.get().setParameterValue(parameter.id, newVal, true);
			pendingAmount = 0;
			lastQuantizedValue = newVal;
		}
	}

	public void finish() {
	}

	public void setToDefault() {
		// todo
	}

	public void inc(boolean fine) {
		// todo
	}

	public void dec(boolean fine) {
		// todo
	}
}