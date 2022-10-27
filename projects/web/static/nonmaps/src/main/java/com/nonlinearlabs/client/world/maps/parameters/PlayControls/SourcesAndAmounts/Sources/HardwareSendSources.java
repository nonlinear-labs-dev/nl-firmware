package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.dataModel.editBuffer.ParameterFactory;
import com.nonlinearlabs.client.world.maps.MapsControl;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

public class HardwareSendSources extends ParameterColumn {

	private PhysicalControlSendParameter[] params = new PhysicalControlSendParameter[ParameterFactory.getNumberOfHW()];

	public HardwareSendSources(MapsLayout parent) {
		super(parent);
        for(int i = 0; i < ParameterFactory.sendParams.length; i++)
        {
            addChild(params[i] = new PhysicalControlSendParameter(this, ParameterFactory.sendParams[i]) {});
        }
	}

    @Override
	public boolean skipChildOnLayout(MapsControl c) {
		return false;
	}
}
