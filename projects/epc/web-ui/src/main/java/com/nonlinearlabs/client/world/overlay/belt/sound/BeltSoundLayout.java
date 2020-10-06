package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.Belt;

public class BeltSoundLayout extends OverlayLayout {

	/*
	 * private LockSymbol lock;
	 */

	private OverlayLayout child;

	public BeltSoundLayout(Belt belt) {
		super(belt);

		child = addChild(createLayout(EditBufferModel.SoundType.Single));
		EditBufferModel.get().soundType.onChange(t -> adaptToSoundType(t));
	}

	boolean adaptToSoundType(EditBufferModel.SoundType t) {
		OverlayLayout newChild = createLayout(t);
		replaceChild(child, newChild);
		child = newChild;
		return true;
	}

	OverlayLayout createLayout(EditBufferModel.SoundType t) {
		switch (t) {
		case Single:
			return new SingleSoundLayout(this);

		case Split:
			return new SplitSoundLayout(this);

		case Layer:
			return new LayerSoundLayout(this);
		}
		return null;
	}

	@Override
	public Control doubleClick(Position p) {
		GWT.log("BeltLayout!");
		return this;
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		child.doLayout(0, 0, w, h);
	}

}