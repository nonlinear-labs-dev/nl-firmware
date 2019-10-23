package com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;
import com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;

class MacrosControls extends ParameterGroupControls {

	private MacrosCol1 col1;
	private MacrosCol2 col2;
	private MacrosCol3 col3;
	private MacrosCol4 col4;

	MacrosControls(MapsLayout parent) {
		super(parent);

		col1 = addChild(new MacrosCol1(this));
		col2 = addChild(new MacrosCol2(this));
		col3 = addChild(new MacrosCol3(this));
		col4 = addChild(new MacrosCol4(this));
	}

	MacroControlParameter getControl(MacroControls mc) {
		switch (mc) {
		case A:
			return col1.getParameter();

		case B:
			return col2.getParameter();

		case C:
			return col3.getParameter();

		case D:
			return col4.getParameter();

		default:
			break;

		}
		return null;
	}
}
