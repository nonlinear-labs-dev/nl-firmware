package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

abstract public class PhysicalControlParameter extends Parameter {

	public PhysicalControlParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new LabelLarge(this));
		addChild(new SliderHorizontalWithHandle(this, getParameterID()));
		addChild(new ValueDisplaySmall(this, getParameterID()));
	}

	@Override
	public String getParameterGroupID() {
		return "CS";
	}

	@Override
	public boolean shouldHaveHandleOnly() {
		return true;
	}

	@Override
	protected int getBackgroundRoundings() {
		return Rect.ROUNDING_NONE;
	}

	@Override
	public boolean dimHandleAtDefaultValue() {
		switch (getReturnMode()) {
		case Center:
			return true;

		case None:
			return false;

		case Zero:
			return true;

		}
		return super.dimHandleAtDefaultValue();
	}

	public enum ReturnMode {
		None, Center, Zero;

		public String toString() {
			switch (this) {
			case None:
				return "None";

			case Zero:
				return "Zero";

			case Center:
				return "Center";

			default:
				return "";
			}
		}

		public static ReturnMode fromInt(int i) {
			for (ReturnMode m : ReturnMode.values())
				if (m.ordinal() == i)
					return m;
			return ReturnMode.None;
		}
	}

	public abstract ReturnMode getReturnMode();

	@Override
	public String getGroupName() {
		return "Hardware Source";
	}
}