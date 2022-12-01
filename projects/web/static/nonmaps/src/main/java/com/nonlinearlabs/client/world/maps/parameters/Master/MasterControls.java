package com.nonlinearlabs.client.world.maps.parameters.Master;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.presenters.EditBufferPresenter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroupControls;

class MasterControls extends ParameterGroupControls {

	MasterControls(MapsLayout parent) {
		super(parent);
		addChild(new MasterCol0(this));
		addChild(new MasterCol1(this));

		EditBufferPresenterProvider.get().onChange(t -> {
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return true;
		});
	}

	private boolean isSingle()
	{
		return EditBufferPresenterProvider.getPresenter().soundType == SoundType.Single;
	}

	@Override
	public double getLeftMargin() {
		return isSingle() ? super.getLeftMargin() : 1;
	}
}
