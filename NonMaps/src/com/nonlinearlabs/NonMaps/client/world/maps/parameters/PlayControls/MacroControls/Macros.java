package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls;

import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;

public class Macros extends ParameterGroupVertical {

	public enum MacroControls {
		NONE, A, B, C, D;

		public String toString() {
			return Integer.toString(ordinal());
		}

		public int toInt() {
			return ordinal();
		}

		public static MacroControls fromInt(int i) {
			for (MacroControls c : values())
				if (c.ordinal() == i)
					return c;

			return NONE;
		}
	}

	private MacrosControls controls;

	public Macros(MapsLayout parent) {
		super(parent);
		addChild(new LabelModuleHeader(this, getName()));
		controls = addChild(new MacrosControls(this));
	}

	public MacroControlParameter getControl(MacroControls mc) {
		return controls.getControl(mc);
	}

	@Override
	public String getID() {
		return "MCs";
	}
}
