package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.SizeLinkedParameterColumn;

class RingModColumn extends SizeLinkedParameterColumn {

	Parameter sizeReference = null;
	Parameter ringMod = null;

	RingModColumn(MapsLayout parent, Parameter sizeReference) {
		super(parent);
		ringMod = addChild(new ModulateableKnob(this, 111));
		this.sizeReference = sizeReference;
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		parentsHeightFromFirstPass = sizeReference.getNonPosition().getHeight();
		super.doSecondLayoutPass(parentsWidthFromFirstPass, parentsHeightFromFirstPass);
		setNonSize(getNonPosition().getWidth(), parentsHeightFromFirstPass);
		LayoutResizingVertical param = (LayoutResizingVertical) getChildren().get(0);
		param.setNonSize(getNonPosition().getDimension().getWidth(), parentsHeightFromFirstPass);
	}
}
