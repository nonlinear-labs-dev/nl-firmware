package com.nonlinearlabs.client.useCases;

import java.util.function.Consumer;

import com.nonlinearlabs.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;

public class IncrementalChanger {
	private double lastQuantizedValue = 0;
	private double pendingAmount = 0;
	private double pixPerRange = 0;
	private double defaultValue = 0;
	private int coarseNumSteps;
	private int fineNumSteps;
	private boolean isBipolar;
	private boolean isBoolean;
	private Consumer<Double> callback;

	IncrementalChanger(DoubleDataModelEntity value, int coarseSteps, int fineSteps, boolean bipolar, boolean isBoolean,
			double pixPerRange, double defaultValue, Consumer<Double> cb) {
		this.coarseNumSteps = coarseSteps;
		this.fineNumSteps = fineSteps;
		this.lastQuantizedValue = value.getValue();
		this.pixPerRange = pixPerRange;
		this.isBipolar = bipolar;
		this.isBoolean = isBoolean;
		this.defaultValue = defaultValue;
		this.callback = cb;
	}

	public IncrementalChanger(ValueDataModelEntity value, double pixelsPerRange, Consumer<Double> cb) {
		this(value.value, value.metaData.coarseDenominator.getValue(), value.metaData.fineDenominator.getValue(),
				value.metaData.bipolar.getBool(), value.metaData.isBoolean.getBool(), pixelsPerRange,
				value.metaData.defaultValue.getValue(), cb);
	}

	public double getQuantizedValue(double v, boolean fine) {
		double steps = fine ? fineNumSteps : coarseNumSteps;
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
		return isBipolar ? -1.0 : 0.0;
	}

	public void changeBy(boolean fine, double amount) {

		amount /= pixPerRange;

		if (isBipolar)
			amount *= 2;

		if (fine)
			amount = amount * coarseNumSteps / fineNumSteps;

		pendingAmount += amount;

		double newVal = getQuantizedValue(lastQuantizedValue + pendingAmount, fine);
		newVal = clip(newVal);

		if (newVal != lastQuantizedValue) {

			if (isBoolean) {
				if (newVal > lastQuantizedValue)
					newVal = 1.0;
				else if (newVal < lastQuantizedValue)
					newVal = 0.0;
			}

			callback.accept(newVal);
			pendingAmount = 0;
			lastQuantizedValue = newVal;
		}
	}

	public void finish() {
	}

	public void setToDefault() {
		callback.accept(defaultValue);
		lastQuantizedValue = defaultValue;
	}

	public void inc(boolean fine) {
		incDec(fine, 1);
	}

	public void dec(boolean fine) {
		incDec(fine, -1);
	}

	protected boolean isValueCoarseQuantized() {
		return getQuantizedClippedValue(false) == getQuantizedClippedValue(true);
	}

	private double getQuantizedClippedValue(boolean fine) {
		return clip(getQuantizedValue(lastQuantizedValue, fine));
	}

	private void incDec(boolean fine, int inc) {
		if (!fine && !isValueCoarseQuantized()) {
			double fineValue = getQuantizedClippedValue(true);
			double coarseValue = getQuantizedClippedValue(false);

			if (coarseValue < fineValue && inc == -1) {
				inc = 0;
			} else if (coarseValue > fineValue && inc == 1) {
				inc = 0;
			}
		}

		double controlVal = clip(lastQuantizedValue);
		double denominator = fine ? fineNumSteps : coarseNumSteps;
		double unRounded = controlVal * denominator;
		double rounded = Math.round(unRounded);
		double newValue = clip((rounded + inc) / denominator);
		callback.accept(newValue);
	}
}