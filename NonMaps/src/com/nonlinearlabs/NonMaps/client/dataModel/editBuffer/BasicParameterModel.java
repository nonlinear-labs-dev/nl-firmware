package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.Notifier;
import com.nonlinearlabs.NonMaps.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;
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
		double roundedVal = (int) (value.value.getValue() * 1000);
		roundedVal /= 1000.0;
		double roundedOgVal = (int) (originalValue.getValue() * 1000);
		roundedOgVal /= 1000.0;
		return roundedVal != roundedOgVal;
	}

	public Updater getUpdater(Node c) {
		return new ParameterUpdater(c, this);
	}

}
