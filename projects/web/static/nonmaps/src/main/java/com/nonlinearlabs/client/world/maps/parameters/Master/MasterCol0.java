package com.nonlinearlabs.client.world.maps.parameters.Master;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.presenters.EditBufferPresenter;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ABSlider;
import com.nonlinearlabs.client.world.maps.parameters.InvertedYModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

public class MasterCol0 extends ParameterColumn {

    private class FB_Mix extends InvertedYModulatableParameter {

		private FB_Mix(MapsLayout parent) {
			super(parent, "FB Mix I-II", 428);
			addChild(new ModulationSourceLabel(this, getParameterNumber()));
			addChild(new ABSlider(this, getParameterNumber()));
			addChild(new ValueDisplayLarge(this, getParameterNumber()));
		}
	}

    public MasterCol0(MapsLayout parent) {
        super(parent);
        addChild(new FB_Mix(this));
    
        EditBufferPresenterProvider.get().onChange(p -> {
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
            return true;
        });
    }
    
    private boolean isSingle() {
        return EditBufferPresenterProvider.getPresenter().soundType == SoundType.Single;
    }

    @Override
	public double getPadding() {
		return isSingle() ? 2 : 0;
	}

	@Override
	public double getYMargin() {
		return isSingle() ? 2 : 0;
	}

    @Override
	public double getBottomMargin() {
        if(isSingle())
		    return super.getBottomMargin() + 1;
        return 0;
    }

    @Override
    public double getXMargin() {
        return isSingle() ? 2 : -2;
    }

    @Override
    public double getWidthMargin()
    {
        return isSingle() ? super.getWidthMargin() : 0;
    }
}
