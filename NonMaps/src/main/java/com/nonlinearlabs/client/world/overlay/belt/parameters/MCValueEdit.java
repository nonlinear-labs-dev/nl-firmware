package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;

public class MCValueEdit extends MCSomething {

	public MCValueEdit(BeltParameterLayout parent) {
		super(parent);
		addChild(middle = new MCValueEditLabel(this));
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		Parameter q = getMacroControl();

		if (q != null) {
			getParent().setMode(BeltParameterLayout.Mode.mcValue);
			getParent().setValueChanger(q.getValue().startUserEdit(getPixRect().getWidth()));
		}

		return getParent();
	}

	@Override
	public Control doubleClick() {
		Parameter q = getMacroControl();

		if (q != null)
			q.setDefault(Parameter.);

		return this;
	}

	public Parameter getMacroControl() {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			MacroControls s = m.getModulationSource();
			if (s != MacroControls.NONE) {
				Parameter q = getNonMaps().getNonLinearWorld().getParameterEditor().getMacroControls().getControl(s);
				return q;
			}
		}
		return null;
	}

	@Override
	protected void inc(ModulatableParameter m, boolean fine) {
		getMacroControl().inc(fine);
	}

	@Override
	protected void dec(ModulatableParameter m, boolean fine) {
		getMacroControl().dec(fine);
	}
}
