package com.nonlinearlabs.client.world.maps.parameters.OscB;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ModulateableKnob;
import com.nonlinearlabs.client.world.maps.parameters.ModulationSourceSwitch;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

public class PhaseColumn extends ParameterColumn {

    public PhaseColumn(MapsLayout parent) {
        super(parent);
		addChild(new ModulateableKnob(this, 302));
		addChild(new ModulationSourceSwitch(this, 394));
    }
    
}