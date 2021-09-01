package com.nonlinearlabs.client.world.maps.parameters.MonoAndUnison;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSlider;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.ParameterGroup;
import com.nonlinearlabs.client.world.maps.parameters.SmallModulateableParameter;

public class UnisonColumn extends ParameterColumn {
      
    public UnisonColumn(ParameterGroup parent) {
        super(parent);
        addChild(new SmallModulateableParameter(this, 249) {
            @Override
            protected int getBackgroundRoundings() {
                return Rect.ROUNDING_NONE;
            }
        });
        addChild(new SmallModulateableParameter(this, 250));
        addChild(new ModulationSourceSlider(this, 252));
        addChild(new ModulationSourceSlider(this, 253));
    }

}
