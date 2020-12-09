package com.nonlinearlabs.client.world.overlay.belt.fadeeditor;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.FadeEditorPresenter;
import com.nonlinearlabs.client.presenters.FadeEditorPresenterProvider;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public abstract class KeyBed extends SVGImage {

    static public double HANDLE_SIZE = 20;

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
        int keyW = 13;
        int keyPadding = 1;
        return note * (keyW + keyPadding);
    }

    public double getXPosFadeRange(double noteF) {
        int keyW = 13;
        int keyPadding = 1;
        return noteF * (keyW + keyPadding);
    }

    public double quantizeToNoteBorder(double splitValue, VoiceGroup vg) {
        int totalKeys = 61;

        int keyW = 13;
        int keyPadding = 1;

        int key = (int) (splitValue * totalKeys);

        if (vg == VoiceGroup.II && EditBufferModel.get().soundType.getValue() == SoundType.Split && key >= 60) {
            return (key * keyW) + (key * keyPadding);
        }
        return (key * keyW) + (key * keyPadding) + keyW + keyPadding;
    }

    protected void drawHandle(Context2d ctx, boolean focus, Rect r, RGB stroke) {
        RGB color = focus ? stroke.brighter(20) : stroke;
        r.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 2, 1, color, RGB.black());
    }

    protected void drawOctaveLabels(Context2d ctx) {
        double fontHeightInPixels = Millimeter.toPixels(2.5);
        ctx.setTextAlign(TextAlign.LEFT);
        ctx.setTextBaseline(TextBaseline.TOP);
        ctx.setFillStyle(RGB.lightGray().toString());
        ctx.setFont(fontHeightInPixels + "px 'SSP-LW25'");

        for (int i = 0; i < 6; i++) {
            Rect r = getPixRect().copy();
            r.moveBy(i * 13 * 12.93, 0);
            r.setWidth(13);
            r.setTop(getSelectedImage().getPixRect().getBottom());
            ctx.fillText("C" + (i + 1), r.getLeft() + 3, r.getTop() + Millimeter.toPixels(3));
        }
    }

    public void drawUnclipped(Context2d ctx) {
    }

    public double getOverflowPixels() {
        return HANDLE_SIZE / 2;
    }
}
