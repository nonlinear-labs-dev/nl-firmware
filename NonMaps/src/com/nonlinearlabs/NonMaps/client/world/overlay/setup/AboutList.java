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
	private String text = "Development:\n\nStephan Schmitt, Daniel Tzschentke, Simone Fabbri, Henry Högelow, Matthias Seeber, Pascal Hürst, Tino Meinert, Gabriel Mulzer, Mike Daliot, Justus Scheil, Nemanja Nikodijevic, Ulrich Wappler, Anton Schmied, Brian Smith, Klaus Strohhäcker, Paul Schulz";
	
	public AboutList(AboutSettingsPage aboutSettingsPage) {
		super(aboutSettingsPage);
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		getPixRect().drawRoundedArea(ctx, Millimeter.toPixels(1), Millimeter.toPixels(0.25), new Gray(19), Gray.floatingWindowHeaderBorder());

		Rect r = getPixRect().getReducedBy(Millimeter.toPixels(2));

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
