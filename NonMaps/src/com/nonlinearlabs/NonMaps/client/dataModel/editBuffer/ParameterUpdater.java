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
		String longName = getChildText(root, "long-name");		
		if (!shortName.isEmpty())
			target.shortName.setValue(shortName);

		if (!longName.isEmpty())
			target.longName.setValue(longName);
		
		
		ValueUpdater vu = new ValueUpdater(root, target.value);
		vu.doUpdate();
	}

}
