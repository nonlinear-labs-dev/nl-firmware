package com.nonlinearlabs.client.dataModel.value;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.world.maps.parameters.Parameter.Initiator;

public class QuantizedClippedValue extends ClippedValue {

	public interface ChangeListener {
		public void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue);

		public void onQuantizedValueChanged(Initiator initiator, double oldQuantizedValue, double newQuantizedValue);

		public void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue);
	}

	private int coarseDenominator = 100;
	private int fineDenominator = 1000;
	private ChangeListener listener = null;

	public QuantizedClippedValue(ChangeListener listener) {
		this.listener = listener;
	}

	public double getQuantizedClipped() {
		return getQuantizedClippedValue(true);
	}

	public double getQuantizedUnClipped() {
		return getQuantizedUnClippedValue(true);
	}

	public int getCoarseDenominator() {
		return coarseDenominator;
	}

	public void setCoarseDenominator(double d) {
		this.coarseDenominator = (int) d;
	}

	public int getFineDenominator() {
		return fineDenominator;
	}

	public void setFineDenominator(double fineDenominator) {
		this.fineDenominator = (int) fineDenominator;
	}

	private double getQuantizedClippedValue(boolean fine) {
		return getQuantizedValue(getClippedValue(), fine);
	}

	private double getQuantizedUnClippedValue(boolean fine) {
		return getQuantizedValue(getRawValue(), fine);
	}

	public double getQuantizedValue(double v, boolean fine) {
		double steps = fine ? fineDenominator : coarseDenominator;
		v *= steps;
		v = Math.round(v);
		return v / steps;
	}

	public void inc(Initiator initiator, boolean fine) {
		incDec(initiator, fine, 1);
	}

	public void dec(Initiator initiator, boolean fine) {
		incDec(initiator, fine, -1);
	}

	private void incDec(Initiator initiator, boolean fine, int inc) {
		if (!fine && !isValueCoarseQuantized()) {
			double fineValue = getQuantizedClippedValue(true);
			double coarseValue = getQuantizedClippedValue(false);

			if (coarseValue < fineValue && inc == -1) {
				inc = 0;
			} else if (coarseValue > fineValue && inc == 1) {
				inc = 0;
			}
		}

		double controlVal = getClippedValue();
		double denominator = fine ? fineDenominator : coarseDenominator;
		double unRounded = controlVal * denominator;
		double rounded = Math.round(unRounded);
		double newValue = clip((rounded + inc) / denominator);
		setRawValue(initiator, newValue);
	}

	protected boolean isValueCoarseQuantized() {
		return getQuantizedClippedValue(false) == getQuantizedClippedValue(true);
	}

	public void applyModulation(Initiator initiator, double delta) {
		if (isBipolar())
			delta *= 2;

		setRawValue(initiator, getRawValue() + delta);
	}

	public void update(Node child) {
		super.update(child);

		String nodeName = child.getNodeName();

		try {
			String value = ServerProxy.getText(child);

			if (nodeName.equals("coarse-denominator")) {
				setCoarseDenominator(Double.parseDouble(value));
			} else if (nodeName.equals("fine-denominator")) {
				setFineDenominator(Double.parseDouble(value));
			}
		} catch (Exception e) {
		}
	}

	@Override
	protected void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
		listener.onRawValueChanged(initiator, oldRawValue, newRawValue);
		super.onRawValueChanged(initiator, oldRawValue, newRawValue);
	}

	@Override
	protected void onClippedValueChanged(Initiator initiator, double oldClippedValue, double newClippedValue) {
		listener.onClippedValueChanged(initiator, oldClippedValue, newClippedValue);

		double oldFine = getQuantizedValue(oldClippedValue, true);
		double newFine = getQuantizedValue(newClippedValue, true);

		if (oldFine != newFine)
			onFineQuantizedChanged(initiator, oldFine, newFine);

		super.onClippedValueChanged(initiator, oldClippedValue, newClippedValue);
	}

	private void onFineQuantizedChanged(Initiator initiator, double oldFine, double newFine) {
		listener.onQuantizedValueChanged(initiator, oldFine, newFine);
	}

	/*-public void update(ValueDataModelEntity e) {
		coarseDenominator = e.metaData.coarseDenominator.getValue();
		fineDenominator = e.metaData.fineDenominator.getValue();
		super.update(e);
	}-*/

}
