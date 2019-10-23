package com.nonlinearlabs.client.world.maps.parameters;

import com.nonlinearlabs.client.world.maps.MapsLayout;

public class Spacer extends ZoomReactingControl {

	private double width;
	private double height;

	public Spacer(MapsLayout parent, double width, double height) {
		super(parent);
		this.width = width;
		this.height = height;
	}

	public Spacer(MapsLayout parent) {
		this(parent, 10, 10);
	}

	@Override
	protected double getBasicWidth() {
		return width;
	}

	@Override
	protected double getBasicHeight() {
		return height;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}
}
