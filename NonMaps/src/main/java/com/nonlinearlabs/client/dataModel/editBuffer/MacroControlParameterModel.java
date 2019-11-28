package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.StringDataModelEntity;
import com.nonlinearlabs.client.dataModel.Updater;

public class MacroControlParameterModel extends BasicParameterModel {
	public StringDataModelEntity givenName = new StringDataModelEntity();
	public StringDataModelEntity info = new StringDataModelEntity();

	public MacroControlParameterModel(ParameterId id) {
		super(id);

		givenName.onChange(e -> notifyChanges());
	}

	public ParameterId[] getRouterIDs() {
		return new ParameterId[] { 
			new ParameterId(id.getNumber() + 12, id.getVoiceGroup()),
			new ParameterId(id.getNumber() + 17, id.getVoiceGroup()),
			new ParameterId(id.getNumber() + 22, id.getVoiceGroup()),
			new ParameterId(id.getNumber() + 27, id.getVoiceGroup()),
			new ParameterId(id.getNumber() + 32, id.getVoiceGroup()),
			new ParameterId(id.getNumber() + 37, id.getVoiceGroup()),
			new ParameterId(id.getNumber() + 42, id.getVoiceGroup()),
			new ParameterId(id.getNumber() + 47, id.getVoiceGroup())};
	}

	@Override
	public Updater createUpdater(Node c) {
		return new MacroControlParameterModelUpdater(c, this);
	}

}
