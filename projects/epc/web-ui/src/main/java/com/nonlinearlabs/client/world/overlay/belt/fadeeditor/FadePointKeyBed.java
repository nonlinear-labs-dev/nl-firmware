package com.nonlinearlabs.client.world.overlay.belt.fadeeditor;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.contextStates.ClipContext;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.presenters.FadeEditorPresenter.KeyRange;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;
import com.nonlinearlabs.client.world.Rect;

public class FadePointKeyBed extends KeyBed {

    enum SelectedHandle {
        FadePointI, FadePointII, FadeRangeI, FadeRangeII, None
    }

    SelectedHandle selection = SelectedHandle.None;

    public FadePointKeyBed(Control parent) {
        super(parent);
    }

    @Override
    public void draw(Context2d ctx, int invalidationMask) {
        try (ClipContext c = new ClipContext(ctx, this)) {
            super.draw(ctx, invalidationMask);

            if (EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.I) {
                drawLayerPart(ctx, VoiceGroup.II);
                drawLayerPart(ctx, VoiceGroup.I);
                drawFadeHandle(ctx, new VoiceGroup[] { VoiceGroup.II, VoiceGroup.I });
            } else {
                drawLayerPart(ctx, VoiceGroup.I);
                drawLayerPart(ctx, VoiceGroup.II);
                drawFadeHandle(ctx, new VoiceGroup[] { VoiceGroup.I, VoiceGroup.II });
            }
        }
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

        double halfHandle = HANDLE_SIZE / 2;
        pix.applyPadding(0, halfHandle, 0, halfHandle);

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

    void selectControl(SelectedHandle handle) {
        selection = handle;
        invalidate(INVALIDATION_FLAG_UI_CHANGED);
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

    public void drawFadeHandle(Context2d ctx, VoiceGroup[] vgs) {
        for (VoiceGroup vg : vgs) {
            drawFadePointHandle(ctx, vg, presenter.getStrokeColor(vg), presenter.getFillColor(vg));
            drawFadeRangeHandle(ctx, vg, presenter.getStrokeColor(vg), presenter.getFillColor(vg));
        }
    }

    @Override
    public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
        if (selection != SelectedHandle.None) {
            updateCP(newPoint);
            return this;
        }
        return super.mouseDrag(oldPoint, newPoint, fine);
    }

    protected Control handleMouseDownDragStart(Position pos) {
        VoiceGroup first = EditBufferModel.get().voiceGroup.getValue();
        VoiceGroup other = first == VoiceGroup.I ? VoiceGroup.II : VoiceGroup.I;
        VoiceGroup vgs[] = { first, other };

        for (VoiceGroup vg : vgs) {
            if (getLayerFadePointRect(vg).contains(pos)) {
                selectControl(vg == VoiceGroup.I ? SelectedHandle.FadePointI : SelectedHandle.FadePointII);
                return this;
            }

            if (getLayerFadeRangeHandleRect(vg).contains(pos)) {
                selectControl(vg == VoiceGroup.I ? SelectedHandle.FadeRangeI : SelectedHandle.FadeRangeII);
                return this;
            }

        }

        selectControl(SelectedHandle.None);
        return null;
    }

    public void updateCP(Position p) {
        try {
            double cp = getCPForPosition(p);
            switch (selection) {
                case FadePointI:
                    EditBufferUseCases.get().setParameterValue(new ParameterId(396, VoiceGroup.I), cp, true);
                    break;
                case FadePointII:
                    EditBufferUseCases.get().setParameterValue(new ParameterId(396, VoiceGroup.II), cp, true);
                    break;
                case FadeRangeI:
                    EditBufferUseCases.get().setParameterValue(new ParameterId(397, VoiceGroup.I), cp, true);
                    break;
                case FadeRangeII:
                    EditBufferUseCases.get().setParameterValue(new ParameterId(397, VoiceGroup.II), cp, true);
                    break;
                case None:
                default:
                    break;
            }
        } catch (Exception e) {
            // expected!
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
        double size = HANDLE_SIZE;
        double halfSize = size / 2;
        Rect pix = getPixRect();
        return new Rect(getFadePointHandleX(vg) - halfSize, pix.getTop(), size, size);
    }

    private Rect getLayerFadeRangeHandleRect(VoiceGroup vg) {
        double size = HANDLE_SIZE;
        double halfSize = size / 2;
        Rect pix = getPixRect();
        return new Rect(getFadeRangeHandleX(vg) - halfSize, pix.getBottom() - size, size, size);
    }

    public double getCPForPosition(Position p) throws Exception {
        Rect pix = getPixRect();
        double xPercent = (p.getX() - pix.getLeft()) / pix.getWidth();

        double fadeI = EditBufferModel.get().getParameter(new ParameterId(396, VoiceGroup.I)).value.value.getValue();
        double fadeII = EditBufferModel.get().getParameter(new ParameterId(396, VoiceGroup.II)).value.value.getValue();

        BasicParameterModel fadeRangeI = EditBufferModel.get().getParameter(new ParameterId(397, VoiceGroup.I));
        BasicParameterModel fadeRangeII = EditBufferModel.get().getParameter(new ParameterId(397, VoiceGroup.II));

        boolean fadeIMin = fadeI <= 0;
        boolean fadeIIMax = fadeII >= 1;

        boolean fine = NonMaps.get().getNonLinearWorld().isShiftDown();

        switch (selection) {
            case FadePointI:
            case FadePointII:
            case FadeRangeI: {
                if (!fadeIMin) {
                    double useableRange = Math.max(0,
                            Math.min(pix.getWidth() - (pix.getWidth() * fadeI), pix.getWidth()));
                    double usableRangePercent = useableRange / pix.getWidth();
                    double newCp = Math.max(0,
                            Math.min((p.getX() - (pix.getLeft() + (pix.getWidth() * fadeI))) / pix.getWidth(),
                                    usableRangePercent));
                    if (fine) {
                        return newCp;
                    } else {
                        return fadeRangeI.value.getQuantizedAndClipped(newCp, false);
                    }
                } else {
                    return xPercent;
                }
            }
            case FadeRangeII: {
                if (!fadeIIMax) {
                    double useableRange = Math.max(0, Math.min((pix.getWidth() * fadeII), pix.getWidth()));
                    double usableRangePercent = useableRange / pix.getWidth();
                    double newCp = Math.max(0,
                            Math.min(((pix.getLeft() + (pix.getWidth() * fadeII)) - p.getX()) / pix.getWidth(),
                                    usableRangePercent));
                    if (fine) {
                        return newCp;
                    } else {
                        return fadeRangeII.value.getQuantizedAndClipped(newCp, false);
                    }
                } else {
                    return 1.0 - xPercent;
                }

            }
            case None:
            default:
                break;

        }

        throw new Exception("out of bounds!");
    }

}
