package com.nonlinearlabs.client.world.maps.parameters.VoiceGroupMaster;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.client.world.maps.parameters.NumericalControlSmall;
import com.nonlinearlabs.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.SliderHorizontal;
import com.nonlinearlabs.client.world.maps.parameters.SliderVertical;
import com.nonlinearlabs.client.world.maps.parameters.UnModulateableParameterName;
import com.nonlinearlabs.client.world.maps.parameters.ValueDisplayLarge;

public class VoiceGroupMasterColumn extends ParameterColumn {
    private class Level extends Parameter {

        private Level(MapsLayout parent) {
            super(parent, 358);
            addChild(new UnModulateableParameterName(this));
			addChild(new SliderVertical(this, getParameterNumber()));
			addChild(new ValueDisplayLarge(this, getParameterNumber()));
        }
    }

    private class UnmodulateablePartMasterParameter extends Parameter {

        private UnmodulateablePartMasterParameter(MapsLayout parent, int num) {
            super(parent, num);
            addChild(new UnModulateableParameterName(this));
            addChild(new NumericalControlSmall(this, getParameterNumber()));
        }
    }

    private class SplitPoint extends ModulatableParameter {
        private SplitPoint(MapsLayout parent) {
            super(parent, 356);
            addChild(new SliderHorizontal(this, 356) {
                @Override 
                public double getTopMargin() {
                    if(presenter.hidden)
                        return 0;
                    return 20;
                }
            });
            addChild(new ValueDisplayLarge(this, 356));
        }
    }

    public VoiceGroupMasterColumn(MapsLayout parent) {
        super(parent);
        addChild(new Level(this));
        addChild(new UnmodulateablePartMasterParameter(this, 360));
        addChild(new UnmodulateablePartMasterParameter(this, 396));
        addChild(new UnmodulateablePartMasterParameter(this, 397));
        addChild(new SplitPoint(this));
    }

}
