package com.nonlinearlabs.client.world.overlay.belt.sound;

import java.util.Arrays;

import com.nonlinearlabs.client.tools.Pair;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.overlay.SVGImage;

class MappedSvgImage<E> extends SVGImage {
	int selectedPhase = 0;
	Pair<E, String> imagesMap[];

	@SafeVarargs
	public MappedSvgImage(Control parent, Pair<E, String>... images)
	{
		super(parent, (String[])Arrays.stream(images).map(x -> x.second).filter(x -> x!=null).toArray());
		imagesMap = images;
	}

	public void update(E e) {
		int phase = 0;

		for(var k: imagesMap) {
			if(k.first == e) {
				setVisible(k.second != null);
				break;
			}

			if(k.second != null)
				phase++;
		}

		if(phase != selectedPhase) {
			selectedPhase = phase;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	@Override
	public int getSelectedPhase() {
		return selectedPhase;
	}
}