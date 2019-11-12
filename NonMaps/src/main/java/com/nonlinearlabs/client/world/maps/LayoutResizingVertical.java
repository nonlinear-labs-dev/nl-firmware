package com.nonlinearlabs.client.world.maps;

public abstract class LayoutResizingVertical extends LayoutVertical {

	public LayoutResizingVertical(MapsLayout parent) {
		super(parent);
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		NonDimension dim = layoutChildren(levelOfDetail);
		setNonSize(dim);
	}

	public void centerChildrenExceptFirst() {
		double height = getNonPosition().getHeight() - getChildren().get(0).getNonPosition().getHeight();
		double childrenHeight = 0;

		for (int i = 1; i < getChildren().size(); i++)
			childrenHeight += getChildren().get(i).getNonPosition().getHeight();

		double diff = (height - childrenHeight) / 2;

		for (int i = 1; i < getChildren().size(); i++)
			getChildren().get(i).getNonPosition().moveBy(0, diff);
	}
}
