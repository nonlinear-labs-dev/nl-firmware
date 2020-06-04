package com.nonlinearlabs.client.world.maps.parameters.FBMixer;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.presenters.EditBufferPresenter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class FBMixerColumn4 extends ParameterColumn {

	FBMixerColumn4(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 160));
		addChild(new ModulatableHorizontalSlider(this, 162) {

			@Override
			protected int getBackgroundRoundings() {
				EditBufferPresenter p = EditBufferPresenterProvider.getPresenter();
				if (p.soundType != SoundType.Layer)
					return Rect.ROUNDING_BOTTOM;

				return Rect.ROUNDING_NONE;
			}

		});
		addChild(new ModulatableHorizontalSlider(this, 354));
	}

}
