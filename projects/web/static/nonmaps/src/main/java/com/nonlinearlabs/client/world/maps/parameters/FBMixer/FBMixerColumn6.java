package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SmallKnobParameter;

class FBMixerColumn6 extends ParameterColumn {

	FBMixerColumn6(MapsLayout parent) {
		super(parent);
		addChild(new SmallKnobParameter(this, 166) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_TOP;
			}
		});
		addChild(new SmallKnobParameter(this, 167) {
			@Override
			protected int getBackgroundRoundings() {
				return Rect.ROUNDING_RIGHT_BOTTOM;
			}
		});
	}

	@Override
	protected boolean shouldDisplayHeaderOnFirstParameter() {
		return false;
	}
}
