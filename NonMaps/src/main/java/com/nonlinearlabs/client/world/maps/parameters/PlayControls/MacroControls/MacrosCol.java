package com.nonlinearlabs.client.world.maps.parameters.PlayControls.MacroControls;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.LabelModulationSource;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceHighPriority;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplaySmall;

abstract class MacrosCol extends ParameterColumn {

	private MacroControlParameter param;
	private int smoothingID;
	private int macroControlID;

	private class MacroControlParameterSlider extends MacroControlParameter {

		private MacroControlParameterSlider(MacrosCol parent, String defName) {
			super(parent, defName);
		}

		@Override
		public int getParameterID() {
			return macroControlID;
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

	private class Smoothing extends ModulationSourceHighPriority {

		private Smoothing(MapsLayout parent) {
			super(parent);
			addChild(new LabelModulationSource(this, getName()));
			addChild(new SliderHorizontal(this));
			addChild(new ValueDisplaySmall(this));
		}

		@Override
		public int getParameterID() {
			return smoothingID;
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

		int diffBetweenMacroControlIDAndItsSmoothing = 81;

		this.macroControlID = macroControlID;
		this.smoothingID = macroControlID + diffBetweenMacroControlIDAndItsSmoothing;
		this.param = new MacroControlParameterSlider(this, defName);

		addChild(param);
		addChild(new Smoothing(this));
	}

	public MacroControlParameter getParameter() {
		return param;
	}
}
