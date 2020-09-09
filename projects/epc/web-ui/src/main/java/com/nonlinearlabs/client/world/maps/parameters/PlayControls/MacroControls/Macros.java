package com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls;

import com.nonlinearlabs.client.dataModel.editBuffer.ModulateableParameterModel.ModSource;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModuleHeader;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;

public class Macros extends ParameterGroup {

	public enum MacroControls {
		NONE, A, B, C, D, E, F;

		public String toString() {
			return Integer.toString(ordinal());
		}

		public String toPrettyString() {
			return toModSource().toString();
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

		public static MacroControls from(ModSource value) {
			switch (value) {
			case A:
				return A;
			case B:
				return B;
			case C:
				return C;
			case D:
				return D;
			case E:
				return E;
			case F:
				return F;
			case None:
				return NONE;
			}
			return NONE;
		}

		public ModSource toModSource() {
			switch (this) {
			case A:
				return ModSource.A;
			case B:
				return ModSource.B;
			case C:
				return ModSource.C;
			case D:
				return ModSource.D;
			case E:
				return ModSource.E;
			case F:
				return ModSource.F;
			case NONE:
				return ModSource.None;
			}
			return ModSource.None;
		}
	}

	private MacrosControls controls;

	public Macros(MapsLayout parent) {
		super(parent, "MCs");
		addChild(new LabelModuleHeader(this) {
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

}
