package com.nonlinearlabs.NonMaps.client.useCases;

import com.nonlinearlabs.NonMaps.client.dataModel.ValueDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;

public class IncrementalChanger {
	private final ValueDataModelEntity theValue;
	private double lastQuantizedValue = 0;
	private double pendingAmount = 0;
	private double pixPerRange = 0;

	IncrementalChanger(ValueDataModelEntity theValue, double pixPerRange) {
		this.theValue = theValue;
		this.lastQuantizedValue = this.theValue.value.getValue();
		this.pixPerRange = pixPerRange;
	}

	public ValueDataModelEntity getValue() {
		return this.theValue;
	}

	public double getQuantizedValue(double v, boolean fine) {
		double steps = fine ? this.theValue.metaData.fineDenominator.getValue()
				: this.theValue.metaData.coarseDenominator.getValue();
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
		return (theValue.metaData.bipolar.getValue() == BooleanValues.on) ? -1.0 : 0.0;
	}

	public void changeBy(boolean fine, double amount) {

		amount /= pixPerRange;

		if (this.theValue.metaData.bipolar.getValue() == BooleanValues.on)
			amount *= 2;

		if (fine)
			amount = amount * this.theValue.metaData.coarseDenominator.getValue()
					/ this.theValue.metaData.fineDenominator.getValue();

		pendingAmount += amount;

		double newVal = getQuantizedValue(lastQuantizedValue + pendingAmount, fine);
		newVal = clip(newVal);

		if (newVal != lastQuantizedValue) {

			if (this.theValue.metaData.isBoolean.getValue() == BooleanValues.on) {
				if (newVal > lastQuantizedValue)
					newVal = 1.0;
				else if (newVal < lastQuantizedValue)
					newVal = 0.0;
			}

			this.theValue.value.setValue(newVal);
			pendingAmount = 0;
			lastQuantizedValue = newVal;
		}
	}

	public void finish() {
	}
}