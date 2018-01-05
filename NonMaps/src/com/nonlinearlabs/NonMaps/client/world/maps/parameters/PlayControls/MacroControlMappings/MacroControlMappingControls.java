package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControlMappings;

import com.nonlinearlabs.NonMaps.client.Checksum;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources.HardwareSourcesCol1;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.HardwareSources.Pedal;

class MacroControlMappingControls extends ParameterGroupControls {

	private HardwareSourcesCol1 sources;
	private MacroControlMappingCol1 mappingCol1;

	MacroControlMappingControls(MapsLayout parent) {
		super(parent);

		addChild(sources = new HardwareSourcesCol1(this));
		addChild(mappingCol1 = new MacroControlMappingCol1(this));
		addChild(new MacroControlMappingCol2(this));
		addChild(new MacroControlMappingCol3(this));
		addChild(new MacroControlMappingCol4(this));
	}

	public void onReturningModeChanged(PhysicalControlParameter src) {
		for (MapsControl c : getChildren()) {
			if (c instanceof MacroControlMappingCol) {
				MacroControlMappingCol col = (MacroControlMappingCol) c;
				col.onReturningModeChanged(src);
			}
		}
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
	}

	public Pedal getPedal(int i) {
		return sources.getPedal(i);
	}

	public boolean isAmountsLocked() {
		return isFirstFoundParameterLocked(mappingCol1);
	}

	public boolean isHWLocked() {
		return isFirstFoundParameterLocked(sources);
	}

	private boolean isFirstFoundParameterLocked(MapsLayout layout) {
		Control c = layout.recurseChildren(new ControlFinder() {
			@Override
			public boolean onWayDownFound(Control ctrl) {
				return (ctrl instanceof Parameter);
			}
		});

		Parameter p = (Parameter) c;
		return p.isLocked();
	}
}
