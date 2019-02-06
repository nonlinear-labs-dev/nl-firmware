package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.NonMaps.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.Notifier;
import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.ValueDataModelEntity;

public class BasicParameterModel extends Notifier<BasicParameterModel> {

	public ValueDataModelEntity value = new ValueDataModelEntity();
	public StringDataModelEntity shortName = new StringDataModelEntity();
	public StringDataModelEntity longName = new StringDataModelEntity();
	public DoubleDataModelEntity originalValue = new DoubleDataModelEntity();

	public BasicParameterModel() {
		value.onChange(e -> notifyChanges());
		shortName.onChange(e -> notifyChanges());
		longName.onChange(e -> notifyChanges());
		originalValue.onChange(e -> notifyChanges());
	}

	@Override
	public BasicParameterModel getValue() {
		return this;
	}

	public boolean isChanged() {
		GWT.log(longName.getValue() + " Compare: " + value.value.getValue() + " - " + originalValue.getValue());
		return value.value.getValue() != originalValue.getValue();
	}

}
