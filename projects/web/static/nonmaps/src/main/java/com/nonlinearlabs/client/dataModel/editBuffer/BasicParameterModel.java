package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Checksum;
import com.nonlinearlabs.client.dataModel.BooleanDataModelEntity;
import com.nonlinearlabs.client.dataModel.DoubleDataModelEntity;
import com.nonlinearlabs.client.dataModel.Notifier;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;
import com.nonlinearlabs.client.dataModel.ValueDataModelEntity;

public class BasicParameterModel extends Notifier<BasicParameterModel> {

	public ParameterId id;
	public ParameterGroupModel group;
	public ValueDataModelEntity value = new ValueDataModelEntity();
	public StringDataModelEntity shortName = new StringDataModelEntity();
	public StringDataModelEntity longName = new StringDataModelEntity();
	public DoubleDataModelEntity originalValue = new DoubleDataModelEntity();
	public BooleanDataModelEntity locked = new BooleanDataModelEntity();
	public StringDataModelEntity info = new StringDataModelEntity();

	public BasicParameterModel(ParameterId id) {
		this.id = id;
		value.onChange(e -> notifyChanges());
		shortName.onChange(e -> notifyChanges());
		longName.onChange(e -> notifyChanges());
		originalValue.onChange(e -> notifyChanges());
		locked.onChange(e -> notifyChanges());
		info.onChange(e -> notifyChanges());
	}

	@Override
	public BasicParameterModel getValue() {
		return this;
	}

	public Updater createUpdater(Node c) {
		return new BasicParameterModelUpdater(c, this);
	}

	public double getIncDecValue(boolean fine, int inc) {
		return value.getIncDecValue(fine, inc);
	}

	public boolean isLocked() {
		return locked.isTrue();
	}

	public void getHash(Checksum crc) {
		crc.eat(value.getQuantizedAndClipped(true));
	}

	public void setGroup(ParameterGroupModel group) {
		this.group = group;
	}

}
