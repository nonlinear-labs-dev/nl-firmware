package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.tools.Pair;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.overlay.SVGImage;

class MappedSvgImage<E> extends SVGImage {
	int selectedPhase = 0;
	Pair<E, String> imagesMap[];

	public static <E> String[] imageMapToArray(Pair<E, String>... images) {
		int totalLengthNotNull = 0;
		for(int i = 0; i < images.length; i++) {
			if(images[i].second != null)
				totalLengthNotNull++;
		}
		String[] ret = new String[totalLengthNotNull];

		int index = 0;
		for(int i = 0; i < images.length; i++) {
			if(images[i].second != null)
			{
				ret[index++] = images[i].second;
			}
		}
		return ret;
	}

	@SafeVarargs
	public MappedSvgImage(Control parent, Pair<E, String>... images)
	{
		super(parent, imageMapToArray(images));
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