package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;

public class ModulateableParameterModelUpdater extends BasicParameterModelUpdater {

	ModulateableParameterModel target;

	public ModulateableParameterModelUpdater(Node c, ModulateableParameterModel p) {
		super(c, p);
		target = p;
	}

	@Override
	public void doUpdate() {
		super.doUpdate();
		String modSource = getChildText(root, "modSrc");
		String modAmount = getChildText(root, "modAmount");

		String modAmountCoarse = getChildText(root, "mod-amount-coarse");
		String modAmountFine = getChildText(root, "mod-amount-fine");
		String modAmountStringizer = getChildText(root, "mod-amount-stringizer");

		boolean notify = false;

		if (!modSource.isEmpty()) {
			target.modSource.setValue(ModSource.values()[Integer.valueOf(modSource)]);
			notify = true;
		}

		if (!modAmount.isEmpty()) {
			target.modAmount.value.setValue(Double.valueOf(modAmount));
			notify = true;
		}

		if (!modAmountCoarse.isEmpty()) {
			target.modAmount.metaData.coarseDenominator.setValue(Integer.valueOf(modAmountCoarse));
			notify = true;
		}

		if (!modAmountFine.isEmpty()) {
			target.modAmount.metaData.fineDenominator.setValue(Integer.valueOf(modAmountFine));
			notify = true;
		}

		if (!modAmountStringizer.isEmpty()) {
			target.modAmount.metaData.scaling.setValue(modAmountStringizer);
			notify = true;
		}

		if (notify)
			target.notifyChanges();
	}
}
