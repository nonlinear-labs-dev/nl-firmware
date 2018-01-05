package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class AboutListLine extends OverlayLayout {

	private SetupLabel label;

	public AboutListLine(AboutList aboutList, String title, String content) {
		super(aboutList);
		addChild(label = new SetupLabel(this, title + ":\t " + content) {
			@Override
			protected RGB getColorFontForSplit(int i) {
				if (i == 0)
					return new Gray(140);

				return new Gray(210);
			}
		});
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		label.doLayout(0, 0, w, h);
	}

}
