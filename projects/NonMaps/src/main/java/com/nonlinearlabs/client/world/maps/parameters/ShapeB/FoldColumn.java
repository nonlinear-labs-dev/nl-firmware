package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.SizeLinkedParameterColumn;

class FoldColumn extends SizeLinkedParameterColumn {

	private class Fold extends ModulateableKnob {

		private Fold(MapsLayout parent) {
			super(parent, 104);
		}
	}

	FoldColumn(MapsLayout parent) {
		super(parent);
		addChild(new Fold(this));
	}
}
