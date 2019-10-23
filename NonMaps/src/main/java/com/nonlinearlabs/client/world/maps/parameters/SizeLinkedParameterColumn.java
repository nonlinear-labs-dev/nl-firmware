package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class SizeLinkedParameterColumn extends ParameterColumn {

	public SizeLinkedParameterColumn(MapsLayout parent) {
		super(parent);
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		super.doSecondLayoutPass(parentsWidthFromFirstPass, parentsHeightFromFirstPass);
		setNonSize(getNonPosition().getWidth(), parentsHeightFromFirstPass);
		LayoutResizingVertical param = (LayoutResizingVertical) getChildren().get(0);
		param.setNonSize(getNonPosition().getDimension().getWidth(),
				parentsHeightFromFirstPass - getTopMargin() - getBottomMargin());
		param.centerChildrenExceptFirst();
	}
}