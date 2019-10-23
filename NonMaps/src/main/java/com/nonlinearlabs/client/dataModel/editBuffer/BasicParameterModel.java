package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;

public class BasicParameterModel extends Notifier<BasicParameterModel> {

	public int id;
	public ValueDataModelEntity value = new ValueDataModelEntity();
	public StringDataModelEntity shortName = new StringDataModelEntity();
	public StringDataModelEntity longName = new StringDataModelEntity();
	public DoubleDataModelEntity originalValue = new DoubleDataModelEntity();

	public BasicParameterModel(int id) {
		this.id = id;
		value.onChange(e -> notifyChanges());
		shortName.onChange(e -> notifyChanges());
		longName.onChange(e -> notifyChanges());
		originalValue.onChange(e -> notifyChanges());
	}

	@Override
	public BasicParameterModel getValue() {
		return this;
	}

	public boolean isValueChanged() {
		int denominator = value.metaData.fineDenominator.getValue();

		long rVal = Math.round(value.value.getValue() * denominator);
		long rOgVal = Math.round(originalValue.getValue() * denominator);

		return rVal != rOgVal;
	}

	public boolean isChanged() {
		return isValueChanged();
	}

	public Updater getUpdater(Node c) {
		return new ParameterUpdater(c, this);
	}

	

	public double getIncDecValue(boolean fine, int inc) {
		return value.getIncDecValue(fine, inc);
	}

}
