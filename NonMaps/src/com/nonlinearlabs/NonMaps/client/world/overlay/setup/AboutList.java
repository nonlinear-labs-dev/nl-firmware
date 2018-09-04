package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.TextSplitter;

public class AboutList extends OverlayControl {

	private TextSplitter splitter;
	private String text = "Development:\n\nStephan Schmitt, Daniel Tzschentke, Simone Fabbri, Henry Högelow, Matthias Seeber, Pascal Hürst, Justus Scheil, Klaus Strohhäcker, Tino Meinert, Anton Schmied, Gabriel Mulzer, Nemanja Nikodijevic, Mike Daliot, Ulrich Wappler";

	public AboutList(AboutSettingsPage aboutSettingsPage) {
		super(aboutSettingsPage);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {

		Rect r = getPixRect();

		double lineHeight = Millimeter.toPixels(4);
		double fontHeight = lineHeight * 0.8;
		double y = r.getTop();

		ctx.setFont(fontHeight + "px regularfont");
		ctx.setTextAlign(TextAlign.LEFT);
		ctx.setFillStyle(new Gray(229).toString());
		ctx.setTextBaseline(TextBaseline.HANGING);

		if (splitter == null) {
			splitter = new TextSplitter(ctx, r.getWidth(), text);
		}

		if (!text.isEmpty()) {
			for (String line : splitter.getLines()) {
				ctx.fillText(line, r.getLeft(), y);
				y += lineHeight;
			}
		}
	}
}
