package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class ModulationSourceButton extends SVGImage {

	MacroControls which;

	public ModulationSourceButton(ModulationButtons parent, MacroControls which) {
		super(parent, "MC_" + which.name() + "_Enabled.svg", "MC_" + which.name() + "_Disabled.svg");
		this.which = which;
	}

	@Override
	public ModulationButtons getParent() {
		return (ModulationButtons) super.getParent();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		if (isVisible() && getParent().isVisible()) {
			final Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();
			if (p instanceof ModulatableParameter) {
				final ModulatableParameter m = (ModulatableParameter) p;
				if (m.getModulationSource() == which)
					m.setModulationSource(MacroControls.NONE, );
				else
					m.setModulationSource(which, );
			}
			getParent().getParent().onMCSelectionChanged();
			return this;
		}
		return null;
	}

	@Override
	public Control doubleClick() {
		if (isVisible() && getParent().isVisible())
			return this;

		return null;
	}

	@Override
	public int getSelectedPhase() {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();
		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			return (m.getModulationSource() == which) ? 0 : 1;
		}
		return 0;
	}
}
