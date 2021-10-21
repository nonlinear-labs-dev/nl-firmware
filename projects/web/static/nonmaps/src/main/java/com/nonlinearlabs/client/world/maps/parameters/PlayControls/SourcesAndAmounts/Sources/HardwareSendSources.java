package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

public class HardwareSendSources extends ParameterColumn {

	private PhysicalControlSendParameter[] params = new PhysicalControlSendParameter[8];

	public HardwareSendSources(MapsLayout parent) {
		super(parent);
        for(int i = 0; i < 8; i++)
        {
            addChild(params[i] = new PhysicalControlSendParameter(this, 1000 + i) {});
        }
	}
}
