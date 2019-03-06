package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Updater;
import com.nonlinearlabs.NonMaps.client.dataModel.ValueUpdater;

public class ParameterUpdater extends Updater {

	protected BasicParameterModel target;

	public ParameterUpdater(Node c, BasicParameterModel p) {
		super(c);
		target = p;
	}

	public void doUpdate() {
		String shortName = getChildText(root, "short-name");
		String longName = getChildText(root, "short-name");
		String og = getChildText(root, "og-value");
		
		if (!shortName.isEmpty())
			target.shortName.setValue(shortName);

		if (!longName.isEmpty())
			target.longName.setValue(longName);
		
		if(!og.isEmpty()) {
			target.originalValue.setValue(Double.valueOf(og));
		}
		
		ValueUpdater vu = new ValueUpdater(root, target.value);
		vu.doUpdate();
	}

}
