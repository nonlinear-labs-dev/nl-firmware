package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.parameters.ParameterColumn;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.GWT;

public class HardwareSourcesCol1 extends ParameterColumn {

	private class Pitchbend extends PhysicalControlParameter {

		private Pitchbend(MapsLayout parent) {
			super(parent, 274);
		}
	}

	private class Aftertouch extends PhysicalControlParameter {

		private Aftertouch(MapsLayout parent) {
			super(parent, 279);
		}
	}

	private Pedal[] pedal = new Pedal[4];

	public HardwareSourcesCol1(MapsLayout parent) {
		super(parent);
		addChild(pedal[0] = new Pedal(this, 254) {
			@Override
			protected int getBackgroundRoundings() {
				//TODO change to Rounding none if local disabled
				return Rect.ROUNDING_LEFT_TOP;
			}
		});
		addChild(pedal[1] = new Pedal(this, 259));
		addChild(pedal[2] = new Pedal(this, 264));
		addChild(pedal[3] = new Pedal(this, 269));
		addChild(new Pitchbend(this));
		addChild(new Aftertouch(this));
		addChild(new Ribbon(this, 284));
		addChild(new Ribbon(this, 289) {
			@Override
			protected int getBackgroundRoundings() {
				//TODO change to Rounding none if local disabled
				return Rect.ROUNDING_LEFT_BOTTOM;
			}
		});
	}

	public Pedal getPedal(int i) {
		return pedal[i];
	}
}
