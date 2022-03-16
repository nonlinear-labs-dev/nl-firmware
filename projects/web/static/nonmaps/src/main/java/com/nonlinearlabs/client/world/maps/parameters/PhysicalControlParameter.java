package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;
import com.nonlinearlabs.client.world.maps.NonRect;

abstract public class PhysicalControlParameter extends Parameter {


	private HardwareCCDisplay ccDisplay;

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
		ccDisplay = addChild(new HardwareCCDisplay(this, getParameterNumber()));
	}
	
	@Override
	public boolean skipChildOnLayout(MapsControl c) {
		return c instanceof HardwareCCDisplay;
	}

	@Override
	protected NonDimension layoutChildren(double levelOfDetail) {
		NonDimension ret = super.layoutChildren(levelOfDetail);
		layoutCCDisplay(levelOfDetail);
		return ret;
	}

	private void layoutCCDisplay(double levelOfDetail) {
		double totalHeight = getNonPosition().getHeight();
		double ccDisplayHeight = ccDisplay.getNonPosition().getHeight();
		ccDisplay.doFirstLayoutPass(levelOfDetail);
		ccDisplay.moveTo(0, totalHeight - ccDisplayHeight);
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