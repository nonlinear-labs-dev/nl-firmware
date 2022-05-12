package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public abstract class ParameterColumn extends LayoutResizingVertical {

	public ParameterColumn(MapsLayout parent) {
		super(parent);
	}

	@Override
	public double getPadding() {
		return 2;
	}

	@Override
	public double getYMargin() {
		return 2;
	}

	@Override
	public double getBottomMargin() {
		return super.getBottomMargin() + 1;
	}

	@Override
	public boolean skipChildOnLayout(MapsControl c) {
		if (c instanceof Parameter) {
			Parameter p = (Parameter) c;
			return p.presenter.hidden;
		}
		return super.skipChildOnLayout(c);
	}

	protected boolean shouldDisplayHeaderOnFirstParameter() {
		return true;
	}
}