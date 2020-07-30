package com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

abstract class MacrosCol extends ParameterColumn {

	private MacroControlParameter param;

	private class MacroControlParameterSlider extends MacroControlParameter {

		private MacroControlParameterSlider(MacrosCol parent, String defName, int parameterID) {
			super(parent, defName, parameterID);
		}

		@Override
		protected int getBackgroundRoundings() {
			MapsLayout grandPa = getParent().getParent();
			int numChildrenInParent = grandPa.getChildren().size();

			if (grandPa.getChildren().get(0) == MacrosCol.this)
				return Rect.ROUNDING_LEFT_TOP;

			if (grandPa.getChildren().get(numChildrenInParent - 1) == MacrosCol.this)
				return Rect.ROUNDING_RIGHT_TOP;

			return Rect.ROUNDING_NONE;
		}
	}

	private class Smoothing extends ModulationSourceSlider {

		private Smoothing(MapsLayout parent, int parameterID) {
			super(parent, parameterID);
		}

		@Override
		public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
			super.doSecondLayoutPass(parentsWidthFromFirstPass, parentsHeightFromFirstPass);
			double w = getNonPosition().getWidth();

			setNonSize(parentsWidthFromFirstPass, getNonPosition().getHeight());
			double diffX = (parentsWidthFromFirstPass - w) / 2;

			for (MapsControl c : getChildren())
				c.getNonPosition().moveBy(diffX, 0);
		}

		@Override
		protected int getBackgroundRoundings() {
			MapsLayout grandPa = getParent().getParent();
			int numChildrenInParent = grandPa.getChildren().size();

			if (grandPa.getChildren().get(0) == MacrosCol.this)
				return Rect.ROUNDING_LEFT_BOTTOM;

			if (grandPa.getChildren().get(numChildrenInParent - 1) == MacrosCol.this)
				return Rect.ROUNDING_RIGHT_BOTTOM;

			return Rect.ROUNDING_NONE;
		}

	}

	MacrosCol(final MapsLayout parent, String defName, final int macroControlID) {
		super(parent);

		this.param = new MacroControlParameterSlider(this, defName, macroControlID);
		addChild(param);
		addChild(new Smoothing(this, getMacroTimeParameterNumber(macroControlID)));
	}

	private int getMacroTimeParameterNumber(final int macroControlID) {
		if (macroControlID == 369)
			return 370;

		if (macroControlID == 371)
			return 372;

		int diffBetweenMacroControlIDAndItsSmoothing = 81;
		return macroControlID + diffBetweenMacroControlIDAndItsSmoothing;
	}

	public MacroControlParameter getParameter() {
		return param;
	}
}
