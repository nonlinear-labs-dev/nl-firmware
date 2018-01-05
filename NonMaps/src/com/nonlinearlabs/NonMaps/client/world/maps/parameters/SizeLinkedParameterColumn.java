package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class SizeLinkedParameterColumn extends ParameterColumn {

	public SizeLinkedParameterColumn(MapsLayout parent) {
		super(parent);
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		super.doSecondLayoutPass(parentsWidthFromFirstPass, parentsHeightFromFirstPass);
		setNonSize(getNonPosition().getWidth(), parentsHeightFromFirstPass);
		LayoutResizingVertical param = (LayoutResizingVertical) getChildren().get(0);
		param.setNonSize(getNonPosition().getDimension().getWidth(), getNonPosition().getDimension().getHeight() - 2 * getYMargin());
		param.centerChildrenExceptFirst();
	}
}