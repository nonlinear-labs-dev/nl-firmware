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

    private boolean mouseIsDown = false;

    private void drawHandle(Context2d ctx, boolean focus, Rect r, RGB stroke) {
        RGB color = stroke;

        if(mouseIsDown && focus)
            color = stroke.brighter(20);

        r.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 2, 1, color, RGB.black());
    }

    private void drawSplitHandle(Context2d ctx, VoiceGroup vg) {
        Rect handle = getSplitPointHandleRect(vg);
        SelectedHandle focus = vg == VoiceGroup.I ? SelectedHandle.SplitPointI : SelectedHandle.SplitPointII;
        drawHandle(ctx, selection == focus, handle, presenter.getStrokeColor(vg));
    }

    private void drawSplitPart(Context2d ctx, VoiceGroup vg) {
        Rect pix = getPixRect().copy();

        ctx.beginPath();
        ctx.setFillStyle(presenter.getFillColor(vg).toString());
        ctx.setStrokeStyle(presenter.getStrokeColor(vg).toString());

        KeyRange range = presenter.getSplitRange(vg);
        double from = getXPosForNote(range.from);
        double to = getXPosForNote(range.to);

        ctx.moveTo(pix.getLeft() + from, pix.getTop());
        ctx.lineTo(pix.getLeft() + to, pix.getTop());
        ctx.lineTo(pix.getLeft() + to, pix.getBottom());
        ctx.lineTo(pix.getLeft() + from, pix.getBottom());
        ctx.lineTo(pix.getLeft() + from, pix.getTop());

        ctx.fill();
        ctx.stroke();
    }

    private VoiceGroup selectedVoiceGroup() {
        return presenter.lastSelectedPart;
    }

    public void drawSplit(Context2d ctx) {
        VoiceGroup vg = selectedVoiceGroup();

        if (vg == VoiceGroup.I) {
            drawSplitPart(ctx, VoiceGroup.II);
            drawSplitPart(ctx, VoiceGroup.I);

            drawSplitHandle(ctx, VoiceGroup.II);
            drawSplitHandle(ctx, VoiceGroup.I);
        } else {
            drawSplitPart(ctx, VoiceGroup.I);
            drawSplitPart(ctx, VoiceGroup.II);

            drawSplitHandle(ctx, VoiceGroup.I);
            drawSplitHandle(ctx, VoiceGroup.II);
        }
    }

    public double getFadePointHandleX(VoiceGroup vg) {
        Rect pix = getPixRect();
        return pix.getLeft() + getXPosForNote(presenter.getFadePointRange(vg).indicator);
    }

    public double getFadeRangeHandleX(VoiceGroup vg) {
        Rect pix = getPixRect();
        return pix.getLeft() + getXPosFadeRange(presenter.getFadeRangePos(vg));
    }

    public Rect getLayerFadePointRect(VoiceGroup vg) {
        double size = 20;
        double halfSize = size / 2;
        Rect pix = getPixRect();
        return new Rect(getFadePointHandleX(vg) - halfSize, pix.getTop() - halfSize, size, size);
    }

    private Rect getSplitPointHandleRect(VoiceGroup vg) {
        double size = 20;
        double halfSize = size / 2;
        Rect pix = getPixRect();
        double x = pix.getLeft() + getXPosForNote(presenter.getSplitRange(vg).indicator);
        return new Rect(x - halfSize, pix.getCenterPoint().getY() - halfSize, size, size);
    }

    private Rect getLayerFadeRangeHandleRect(VoiceGroup vg) {
        double size = 20;
        double halfSize = size / 2;
        Rect pix = getPixRect();
        return new Rect(getFadeRangeHandleX(vg) - halfSize, pix.getBottom() - halfSize, size, size);
    }

    private void drawFadeRangeHandle(Context2d ctx, VoiceGroup vg, RGB stroke, RGBA fill) {
        Rect handle = getLayerFadeRangeHandleRect(vg);
        SelectedHandle focus = vg == VoiceGroup.I ? SelectedHandle.FadeRangeI : SelectedHandle.FadeRangeII;
        drawHandle(ctx, selection == focus, handle, stroke);
    }

    private void drawFadePointHandle(Context2d ctx, VoiceGroup vg, RGB stroke, RGBA fill) {
        Rect handle = getLayerFadePointRect(vg);
        SelectedHandle focus = vg == VoiceGroup.I ? SelectedHandle.FadePointI : SelectedHandle.FadePointII;
        drawHandle(ctx, selection == focus, handle, stroke);
    }

    private void drawLayerPart(Context2d ctx, VoiceGroup vg) {
        Rect pix = getPixRect().copy();

        ctx.beginPath();
        ctx.setFillStyle(presenter.getFillColor(vg).toString());
        ctx.setStrokeStyle(presenter.getStrokeColor(vg).toString());

        KeyRange range = presenter.getFadePointRange(vg);
        double from = getXPosForNote(range.from);
        double to = getXPosForNote(range.to);
        double toBottom = getXPosFadeRange(presenter.getFadeRangePos(vg));

        if (vg == VoiceGroup.I) {
            ctx.moveTo(pix.getLeft() + from, pix.getTop());
            ctx.lineTo(pix.getLeft() + to, pix.getTop());
            ctx.lineTo(pix.getLeft() + toBottom, pix.getBottom());
            ctx.lineTo(pix.getLeft() + to, pix.getBottom());
            ctx.lineTo(pix.getLeft() + from, pix.getBottom());
            ctx.lineTo(pix.getLeft() + from, pix.getTop());
        } else {
            ctx.moveTo(pix.getLeft() + from, pix.getTop());
            ctx.lineTo(pix.getLeft() + toBottom, pix.getBottom());
            ctx.lineTo(pix.getLeft() + to, pix.getBottom());
            ctx.lineTo(pix.getLeft() + to, pix.getTop());
            ctx.lineTo(pix.getLeft() + from, pix.getTop());
        }

        ctx.fill();
        ctx.stroke();
    }

    public void drawFadeHandle(Context2d ctx, VoiceGroup[] vgs) {
        for (VoiceGroup vg : vgs) {
            drawFadePointHandle(ctx, vg, presenter.getStrokeColor(vg), presenter.getFillColor(vg));
            drawFadeRangeHandle(ctx, vg, presenter.getStrokeColor(vg), presenter.getFillColor(vg));
        }
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
            ctx.fillText("C" + (i + 1), r.getLeft() + 3, r.getTop() + Millimeter.toPixels(2));
        }
    }
}
