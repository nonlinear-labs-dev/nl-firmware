package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class SizeLinkedParameterColumn extends ParameterColumn {

	private MapsControl referenceControl = null;

	public SizeLinkedParameterColumn(MapsLayout parent) {
		super(parent);
	}

	public void setReferenceControl(MapsControl ref) {
		referenceControl = ref;
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		double s = referenceControl != null ? referenceControl.getNonPosition().getHeight() : parentsWidthFromFirstPass;
		super.doSecondLayoutPass(parentsWidthFromFirstPass, s);
		setNonSize(getNonPosition().getWidth(), s);
		LayoutResizingVertical param = (LayoutResizingVertical) getChildren().get(0);
		param.setNonSize(getNonPosition().getDimension().getWidth(), s - getTopMargin() - getBottomMargin());
		param.centerChildrenExceptFirst();
	}
}