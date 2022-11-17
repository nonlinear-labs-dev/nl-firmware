package com.nonlinearlabs.client.world.maps.parameters.EnvC;

import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class EnvCControls extends ParameterGroupControls {

	private EnvCExtraBlock extraBlock;

	EnvCControls(MapsLayout parent) {
		super(parent);

		addChild(new EnvCCol1(this));
		addChild(new EnvCCol2(this));
		addChild(new EnvCCol3(this));
		addChild(new EnvCCol4(this));
		addChild(new EnvCCol5(this));
		addChild(new EnvCCol6(this));

		extraBlock = addChild(new EnvCExtraBlock(this));
	}

	@Override
	public boolean skipChildOnLayout(MapsControl c) {
		return c == extraBlock;
	}

	@Override
	protected NonDimension layoutChildren(double levelOfDetail) {
		NonDimension ret = super.layoutChildren(levelOfDetail);
		extraBlock.moveTo(80 + 20, ret.getHeight() - extraBlock.getNonPosition().getHeight() + 5);
		return new NonDimension(ret.getWidth() - getPadding(), ret.getHeight() + 10);
	}
}
