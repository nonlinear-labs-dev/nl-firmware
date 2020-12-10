package com.nonlinearlabs.client.useCases;

import java.util.function.BiConsumer;

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
	private BiConsumer<Double, Boolean> callback;
	private Runnable doneCallback;

	IncrementalChanger(DoubleDataModelEntity value, int coarseSteps, int fineSteps, boolean bipolar, boolean isBoolean,
			double pixPerRange, double defaultValue, BiConsumer<Double, Boolean> cb, Runnable doneCallback) {
		this.coarseNumSteps = coarseSteps;
		this.fineNumSteps = fineSteps;
		this.lastQuantizedValue = value.getValue();
		this.pixPerRange = pixPerRange;
		this.isBipolar = bipolar;
		this.isBoolean = isBoolean;
		this.defaultValue = defaultValue;
		this.callback = cb;
		this.doneCallback = doneCallback;
	}

	public IncrementalChanger(ValueDataModelEntity value, double pixelsPerRange, BiConsumer<Double, Boolean> cb,
			Runnable doneCallback) {
		this(value.value, value.metaData.coarseDenominator.getValue(), value.metaData.fineDenominator.getValue(),
				value.metaData.bipolar.getBool(), value.metaData.isBoolean.getBool(), pixelsPerRange,
				value.metaData.defaultValue.getValue(), cb, doneCallback);
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
		if (amount == 0)
			return;

		amount /= pixPerRange;

		if (isBipolar)
			amount *= 2;

		if (fine)
			amount = amount * coarseNumSteps / fineNumSteps;

		pendingAmount += bendAmount(amount);

		double newValRaw = getQuantizedValue(lastQuantizedValue + amount, fine);
		double newValPending = getQuantizedValue(lastQuantizedValue + pendingAmount, fine);

		newValPending = clip(newValPending);

		if (newValPending != lastQuantizedValue) {

			if (isBoolean) {
				if (newValPending > lastQuantizedValue)
					newValPending = 1.0;
				else if (newValPending < lastQuantizedValue)
					newValPending = 0.0;
			}

			callback.accept(newValPending, false);
			pendingAmount -= (newValPending - lastQuantizedValue);
			lastQuantizedValue = newValPending;
		} else if (newValRaw != lastQuantizedValue
				&& (lastQuantizedValue == getLowerBorder() || lastQuantizedValue == getUpperBorder())) {
			if (isBoolean) {
				if (newValRaw > lastQuantizedValue)
					newValRaw = 1.0;
				else if (newValRaw < lastQuantizedValue)
					newValRaw = 0.0;
			}

			callback.accept(newValRaw, false);
			pendingAmount = 0;
			lastQuantizedValue = newValRaw;
		}
	}

	public void finish() {
		if (doneCallback != null)
			doneCallback.run();
	}

	public void setToDefault() {
		callback.accept(defaultValue, true);
		lastQuantizedValue = defaultValue;
	}

	public void inc(boolean fine) {
		incDec(fine, bendAmount(1.0) < 0 ? -1 : 1);
	}

	public void dec(boolean fine) {
		incDec(fine, bendAmount(-1.0) < 0 ? -1 : 1);
	}

	public double bendAmount(double i) {
		return i;
	}

	protected boolean isValueCoarseQuantized() {
		return getQuantizedClippedValue(false) == getQuantizedClippedValue(true);
	}

	private double getQuantizedClippedValue(boolean fine) {
		return clip(getQuantizedValue(lastQuantizedValue, fine));
	}

	private void incDec(boolean fine, double inc) {
		if (!fine && !isValueCoarseQuantized()) {
			double fineValue = getQuantizedClippedValue(true);
			double coarseValue = getQuantizedClippedValue(false);

			if (coarseValue < fineValue && inc < 0) {
				inc = 0;
			} else if (coarseValue > fineValue && inc > 0) {
				inc = 0;
			}
		}

		double controlVal = clip(lastQuantizedValue);
		double denominator = fine ? fineNumSteps : coarseNumSteps;
		double unRounded = controlVal * denominator;
		double rounded = Math.round(unRounded);
		double newValue = clip((rounded + inc) / denominator);
		callback.accept(newValue, true);
	}
}