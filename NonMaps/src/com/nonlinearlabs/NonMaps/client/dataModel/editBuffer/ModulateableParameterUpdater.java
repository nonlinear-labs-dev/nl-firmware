package com.nonlinearlabs.NonMaps.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.editBuffer.ModulateableParameter.ModSource;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.MacroControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;

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
		String modAmount = getChildText(root, "modAmount");

		if (!modSource.isEmpty()) {
			modTarget.modSource.setValue(ModSource.values()[Integer.valueOf(modSource)]);
			MacroControlParameter mc = NonMaps.get().getNonLinearWorld().getParameterEditor().getMacroControls()
					.getControl(MacroControls.values()[modTarget.modSource.getValue().ordinal()]);
			if (mc != null)
				modTarget.mcParameterID.setValue(mc.getParameterID());
			modTarget.notifyChanges();
		}

		if (!modAmount.isEmpty()) {
			modTarget.modAmount.setValue(Double.valueOf(modAmount));
			modTarget.notifyChanges();
		}
	}
}
