package com.nonlinearlabs.client.world.maps.parameters.ShapeB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.SizeLinkedParameterColumn;

class FoldColumn extends SizeLinkedParameterColumn {

	FoldColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 104));
	}
}
