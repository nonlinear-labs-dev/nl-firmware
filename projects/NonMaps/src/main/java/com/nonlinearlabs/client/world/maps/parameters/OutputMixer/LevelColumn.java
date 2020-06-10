package com.nonlinearlabs.client.world.maps.parameters.OutputMixer;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.presenters.EditBufferPresenter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

class LevelColumn extends ParameterColumn {

	LevelColumn(MapsLayout parent) {
		super(parent);
		addChild(new ModulatableSlider(this, 185));
		addChild(new ModulationSourceSlider(this, 187) {
			@Override
			protected int getBackgroundRoundings() {
				EditBufferPresenter p = EditBufferPresenterProvider.getPresenter();
				if (p.soundType == SoundType.Single)
					return Rect.ROUNDING_BOTTOM;

				return Rect.ROUNDING_NONE;
			}
		});
		addChild(new ModulatableHorizontalSlider(this, 362));
	}

}
