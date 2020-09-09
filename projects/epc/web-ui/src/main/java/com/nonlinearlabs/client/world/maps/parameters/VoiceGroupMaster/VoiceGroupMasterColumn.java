package com.nonlinearlabs.client.world.maps.parameters.VoiceGroupMaster;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableHorizontalSlider;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameterWithoutName;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceLabel;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderVertical;
import com.nonlinearlabs.client.world.maps.parameters.SmallModulateableParameter;
import com.nonlinearlabs.client.world.maps.parameters.UnModulateableParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

public class VoiceGroupMasterColumn extends ParameterColumn {

    private final class MasterVolume extends ModulatableParameterWithoutName {
        private MasterVolume(MapsLayout parent, int parameterID) {
            super(parent, parameterID);
            addChild(new UnModulateableParameterName(this));
            addChild(new ModulationSourceLabel(this, getParameterNumber()));
            addChild(new SliderVertical(this, getParameterNumber()));
            addChild(new ValueDisplayLarge(this, getParameterNumber()));
        }
    }

    public VoiceGroupMasterColumn(MapsLayout parent) {
        super(parent);
        addChild(new MasterVolume(this, 358));
        addChild(new SmallModulateableParameter(this, 360));
        addChild(new SmallModulateableParameter(this, 396));
        addChild(new SmallModulateableParameter(this, 397) {
            @Override
            protected int getBackgroundRoundings() {
                if (EditBufferPresenterProvider.getPresenter().soundType == SoundType.Layer)
                    return Rect.ROUNDING_BOTTOM;
                return super.getBackgroundRoundings();
            }
        });
        addChild(new ModulatableHorizontalSlider(this, 356));
    }

}
