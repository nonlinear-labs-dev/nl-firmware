package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls;

import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupVertical;

public class Macros extends ParameterGroupVertical {

	public enum MacroControls {
		NONE, A, B, C, D;

		public String toString() {
			return Integer.toString(ordinal());
		}

		public String toPrettyString() {
			switch (this) {
			case A:
				return "\uE000";
			case B:
				return "\uE001";
			case C:
				return "\uE002";
			case D:
				return "\uE003";
			case NONE:
				return "";
			}
			return "";
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
		addChild(new LabelModuleHeader(this, getName()) {
			@Override
			public RGB getColorFont() {
				return RGB.lighterGray();
			}
		});
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
