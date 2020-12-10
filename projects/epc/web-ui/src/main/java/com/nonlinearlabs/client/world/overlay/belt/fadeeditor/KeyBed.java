package com.nonlinearlabs.client.world.overlay.belt.fadeeditor;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.presenters.FadeEditorPresenter;
import com.nonlinearlabs.client.presenters.FadeEditorPresenterProvider;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public abstract class KeyBed extends SVGImage {

    static public double HANDLE_SIZE = 20 * NonMaps.devicePixelRatio;
    static public double KEY_WIDTH = 13 * NonMaps.devicePixelRatio;
    static public double KEY_PADDING = 1 * NonMaps.devicePixelRatio;

    FadeEditorPresenter presenter;

    public KeyBed(Control parent) {
        super(parent, "Keys-Fade-Range.svg");

        FadeEditorPresenterProvider.get().onChange(p -> {
            presenter = p;
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
            return true;
        });
    }

    public double getXPosForNote(int note) {
        return note * (KEY_WIDTH + KEY_PADDING);
    }

    public double getXPosFadeRange(double noteF) {
        return noteF * (KEY_WIDTH + KEY_PADDING);
    }

    protected void drawOctaveLabels(Context2d ctx) {
        double fontHeightInPixels = Millimeter.toPixels(2.5);
        ctx.setTextAlign(TextAlign.LEFT);
        ctx.setTextBaseline(TextBaseline.TOP);
        ctx.setFillStyle(RGB.lightGray().toString());
        ctx.setFont(fontHeightInPixels + "px 'SSP-LW25'");

        for (int i = 0; i < 6; i++) {
            Rect r = getPixRect().copy();
            r.moveBy(i * KEY_WIDTH * 12.93, 0);
            r.setWidth(KEY_WIDTH / NonMaps.devicePixelRatio);
            r.setTop(getSelectedImage().getPixRect().getBottom());
            ctx.fillText("C" + (i + 1), r.getLeft() + 3, r.getTop() + Millimeter.toPixels(3));
        }
    }

    public double getOverflowPixels() {
        return HANDLE_SIZE / 2;
    }
}
