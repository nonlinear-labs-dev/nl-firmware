package com.nonlinearlabs.client.world.maps;

public abstract class LayoutResizingHorizontal extends LayoutHorizontal {

	public LayoutResizingHorizontal(MapsControl parent) {
		super(parent);
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		NonDimension dim = layoutChildren(levelOfDetail);
		setNonSize(dim);
	}
}
