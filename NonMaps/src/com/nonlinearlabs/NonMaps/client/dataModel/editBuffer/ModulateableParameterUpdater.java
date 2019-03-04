package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter.ModSource;

public class ModulateableParameterUpdater extends ParameterUpdater {

	ModulateableParameter modTarget;
	
	public ModulateableParameterUpdater(Node c, ModulateableParameter p) {
		super(c, p);
		modTarget = p;
	}
	
	@Override
	public void doUpdate() {
		super.doUpdate();
		String modSource = getChildText(root, "modSrc");
		String ogModSource = getChildText(root, "og-modSrc");
		String modAmount = getChildText(root, "modAmount");
		String ogModAmount = getChildText(root, "og-modAmount");
		
		if(!modSource.isEmpty()) {
			modTarget.modSource.setValue(ModSource.values()[Integer.valueOf(modSource)]);
			modTarget.notifyChanges();
		}
		
		if(!ogModSource.isEmpty()) {
			modTarget.ogModSource.setValue(ModSource.values()[Integer.valueOf(ogModSource)]);
			modTarget.notifyChanges();
		}
		
		if(!modAmount.isEmpty()) {
			modTarget.modAmount.setValue(Double.valueOf(modAmount));
			modTarget.notifyChanges();
		}
		
		if(!ogModAmount.isEmpty()) {
			modTarget.ogModAmount.setValue(Double.valueOf(ogModAmount));
			modTarget.notifyChanges();
		}
	}

}
