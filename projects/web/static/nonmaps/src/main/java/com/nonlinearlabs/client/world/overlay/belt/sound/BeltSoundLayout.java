package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.Belt;

public class BeltSoundLayout extends OverlayLayout {

	/*
	 * private LockSymbol lock;
	 */

	private OverlayLayout child;

	private OverlayLayout singleSoundLayout;
	private OverlayLayout layerSoundLayout;
	private OverlayLayout splitSoundLayout;

	public BeltSoundLayout(Belt belt) {
		super(belt);


		singleSoundLayout = createLayout(EditBufferModel.SoundType.Single);
		layerSoundLayout = createLayout(EditBufferModel.SoundType.Layer);
		splitSoundLayout = createLayout(EditBufferModel.SoundType.Split);

		child = addChild(getLayout(EditBufferModel.SoundType.Single));
		EditBufferPresenterProvider.get().onChange(ebp -> {
			adaptToSoundType(ebp.soundType);
			return true;
		});
	}

	boolean adaptToSoundType(EditBufferModel.SoundType t) {
		OverlayLayout newChild = getLayout(t);
		replaceChild(child, newChild);
		child = newChild;
		return true;
	}

	OverlayLayout getLayout(EditBufferModel.SoundType t) {
		switch(t) {
			case Single:
				return singleSoundLayout;
			case Split:
				return splitSoundLayout;
			case Layer:
				return layerSoundLayout;
		}
		return null;
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
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		child.doLayout(0, 0, w, h);
	}

}