package com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts;

import com.nonlinearlabs.NonMaps.client.Checksum;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ParameterGroupControls;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts.AmountsCol;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts.AmountsCol1;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts.AmountsCol2;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts.AmountsCol3;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Amounts.AmountsCol4;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.HardwareSourcesCol1;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources.Pedal;

class SourcesAndAmountsControls extends ParameterGroupControls {

	private HardwareSourcesCol1 sources;
	private AmountsCol1 mappingCol1;

	SourcesAndAmountsControls(MapsLayout parent) {
		super(parent);

		addChild(sources = new HardwareSourcesCol1(this));
		addChild(mappingCol1 = new AmountsCol1(this));
		addChild(new AmountsCol2(this));
		addChild(new AmountsCol3(this));
		addChild(new AmountsCol4(this));
	}

	public void onReturningModeChanged(PhysicalControlParameter src) {
		for (MapsControl c : getChildren()) {
			if (c instanceof AmountsCol) {
				AmountsCol col = (AmountsCol) c;
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
