package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;

public class ValueUpdater extends Updater {
	private ValueDataModelEntity target;

	public ValueUpdater(ValueDataModelEntity target) {
		this.target = target;
	}

	public void update(Node node) {
		String coarse = getChildText(node, "coarse-denominator");
		String fine = getChildText(node, "fine-denominator");
		String bipolar = getChildText(node, "bipolar");
		String scaling = getChildText(node, "scaling");
		String defaultValue = getChildText(node, "default");

		if (!bipolar.isEmpty())
			target.metaData.bipolar.setValue(bipolar.equals("1") ? BooleanValues.on : BooleanValues.off);

		if (!coarse.isEmpty())
			target.metaData.coarseDenominator.setValue((int) Double.parseDouble(coarse));

		if (!fine.isEmpty())
			target.metaData.fineDenominator.setValue((int) Double.parseDouble(fine));

		if (!scaling.isEmpty())
			target.metaData.scaling.setValue(scaling);

		if (!defaultValue.isEmpty())
			target.metaData.defaultValue.setValue(Double.parseDouble(defaultValue));

		String value = getChildText(node, "value");
		target.setValue(Double.parseDouble(value));

		target.notifyChanges();
	}
}
