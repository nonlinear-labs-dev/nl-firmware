package com.nonlinearlabs.NonMaps.client.dataModel.value;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;

class Value {
	private double rawValue = 0;
	private double defaultValue = 0;
	private boolean isBipolar = false;
	private boolean isBoolean = false;

	public double getRawValue() {
		return rawValue;
	}

	public void setRawValue(Initiator initiator, double newRawValue) {
		double oldRawValue = rawValue;
		this.rawValue = newRawValue;
		onRawValueChanged(initiator, oldRawValue, newRawValue);
	}

	public double getDefaultValue() {
		return defaultValue;
	}

	public void setDefaultValue(double defaultValue) {
		this.defaultValue = defaultValue;
	}

	public boolean isBipolar() {
		return isBipolar;
	}

	public boolean isBoolean() {
		return isBoolean;
	}

	public void setBipolar(boolean isBipolar) {
		this.isBipolar = isBipolar;
	}

	public void setBoolean(boolean isBoolean) {
		this.isBoolean = isBoolean;
	}

	public void setToDefault(Initiator initiator) {
		setRawValue(initiator, getDefaultValue());
	}

	public void update(Node child) {
		String nodeName = child.getNodeName();

		try {
			String value = ServerProxy.getText(child);

			if (nodeName.equals("value")) {
				setRawValue(Initiator.INDIRECT_USER_ACTION, Double.parseDouble(value));
			} else if (nodeName.equals("bipolar")) {
				setBipolar(value.equals("1"));
			} else if (nodeName.equals("default")) {
				setDefaultValue(Double.parseDouble(value));
			}
		} catch (Exception e) {
		}
	}

	protected void onRawValueChanged(Initiator initiator, double oldRawValue, double newRawValue) {
	}

	protected void onEditingFinished() {

	}
	/*-
	 public void update(ValueDataModelEntity e) {
	 this.isBipolar = e.metaData.bipolar.getValue() == BooleanValues.on;
	 this.defaultValue = e.metaData.defaultValue.getValue();
	 setRawValue(Initiator.INDIRECT_USER_ACTION, e.getValue());
	 }-*/
}
