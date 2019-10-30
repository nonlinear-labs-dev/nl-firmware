package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;

public class MCAmountEdit extends MCSomething {

	public MCAmountEdit(BeltParameterLayout parent) {
		super(parent);
		addChild(middle = new MCAmountEditLabel(this));
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			getParent().setMode(BeltParameterLayout.Mode.mcAmount);
			ModulatableParameter m = (ModulatableParameter) p;
			if (m.getModulationSource() != MacroControls.NONE)
				getParent().setValueChanger(m.getModulationAmount().startUserEdit(getPixRect().getWidth()));
		}

		return getParent();
	}

	@Override
	public Control doubleClick() {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			if (m.getModulationSource() != MacroControls.NONE)
				m.getModulationAmount().setToDefault(Parameter.);
		}

		return this;
	}

	@Override
	protected void inc(ModulatableParameter m, boolean fine) {
		m.getModulationAmount().inc(fine);
	}

	@Override
	protected void dec(ModulatableParameter m, boolean fine) {
		m.getModulationAmount().dec(fine);
	}

}
