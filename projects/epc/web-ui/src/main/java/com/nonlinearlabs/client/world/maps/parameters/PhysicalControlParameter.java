package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;

abstract public class PhysicalControlParameter extends Parameter {

	public PhysicalControlParameter(MapsLayout parent, int parameterID) {
		super(parent, parameterID);
		addChild(new UnModulateableParameterName(this));
		addChild(new SliderHorizontalWithHandle(this, getParameterNumber()));
		addChild(new ValueDisplaySmall(this, getParameterNumber()) {
			@Override
			protected double getFontHeight() {
				return 11;
			}
		
			@Override
			protected double getBasicWidth() {
				return 80;
			}
			
			@Override
			public double getLevelOfDetailForFullVisibility() {
				return 5;
			}
		});
	}

	@Override
	public String getParameterGroupID() {
		return "CS";
	}

	@Override
	protected int getBackgroundRoundings() {
		return Rect.ROUNDING_NONE;
	}

	@Override
	public boolean dimHandleAtDefaultValue() {
		return presenter.isReturning;
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

	@Override
	public String getGroupName() {
		return "Hardware Source";
	}
}