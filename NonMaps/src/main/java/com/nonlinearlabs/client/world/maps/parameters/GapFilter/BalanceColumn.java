package com.nonlinearlabs.client.world.maps.parameters.GapFilter;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class BalanceColumn extends ParameterColumn {

	BalanceColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulateableKnob(this, 207));
	}
}
