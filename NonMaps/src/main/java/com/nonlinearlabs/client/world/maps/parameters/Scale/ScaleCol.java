package com.nonlinearlabs.client.world.maps.parameters.Scale;

import java.util.ArrayList;
import java.util.List;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;

public class ScaleCol extends ParameterColumn {

	public ScaleCol(MapsLayout parent) {
		super(parent);
	}

	public List<ScaleParameter> getScaleParams() {
		List<ScaleParameter> ret = new ArrayList<ScaleParameter>();
		for (Control control : getChildren()) {
			if (control instanceof ScaleParameter) {
				ret.add((ScaleParameter) control);
			}
		}
		return ret;
	}

}
