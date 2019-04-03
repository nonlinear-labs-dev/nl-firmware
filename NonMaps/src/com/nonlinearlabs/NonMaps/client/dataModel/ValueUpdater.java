package com.nonlinearlabs.NonMaps.client.dataModel;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup.BooleanValues;

public class ValueUpdater extends Updater {
	private ValueDataModelEntity target;

	public ValueUpdater(Node root, ValueDataModelEntity target) {
		super(root);
		this.target = target;
	}

	@Override
	public void doUpdate() {
		String coarse = getChildText(root, "coarse-denominator");
		String fine = getChildText(root, "fine-denominator");
		String bipolar = getChildText(root, "bipolar");
		String scaling = getChildText(root, "scaling");
		String defaultValue = getChildText(root, "default");

		if (!bipolar.isEmpty()) {
			BooleanDataModelEntity bp = target.metaData.bipolar;
			bp.setValue(bipolar.equals("1") ? BooleanValues.on : BooleanValues.off);
		}

		if (!coarse.isEmpty())
			target.metaData.coarseDenominator.setValue((int) Double.parseDouble(coarse));

		if (!fine.isEmpty())
			target.metaData.fineDenominator.setValue((int) Double.parseDouble(fine));

		if (!scaling.isEmpty())
			target.metaData.scaling.setValue(scaling);

		if (!defaultValue.isEmpty())
			target.metaData.defaultValue.setValue(Double.parseDouble(defaultValue));

		String value = getChildText(root, "value");
		target.value.setValue(Double.parseDouble(value));

		target.notifyChanges();
	}
}
