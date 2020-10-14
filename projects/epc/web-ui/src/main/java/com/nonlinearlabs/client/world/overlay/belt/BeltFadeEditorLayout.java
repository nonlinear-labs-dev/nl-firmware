package com.nonlinearlabs.client.world.overlay.belt;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.DragProxy;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.world.overlay.belt.Belt.BeltTab;
import com.nonlinearlabs.client.presenters.*;
import com.nonlinearlabs.client.presenters.FadeEditorPresenter.KeyRange;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class BeltFadeEditorLayout extends OverlayLayout {

    enum SelectedAnfasser {
        SplitPointI, SplitPointII, FadePointI, FadePointII, FadeRangeI, FadeRangeII, None
    }

    private class KeyBed extends SVGImage {

        SelectedAnfasser selection = SelectedAnfasser.None;

        FadeEditorPresenter presenter;

        public KeyBed(Control parent) {
            super(parent, "Keys-Fade-Range.svg");

			FadeEditorPresenterProvider.get().onChange(p -> {
                presenter = p;
                invalidate(INVALIDATION_FLAG_UI_CHANGED);
				return true;
			});
        }

        @Override
        public int getSelectedPhase() {
            return 0;
        }

        @Override
        public void draw(Context2d ctx, int invalidationMask) {
            super.draw(ctx, invalidationMask);

            if (EditBufferModel.get().soundType.getValue() == SoundType.Layer) {
                drawLayer(ctx, invalidationMask);
            } else if (EditBufferModel.get().soundType.getValue() == SoundType.Split) {
                drawSplit(ctx, invalidationMask);
            }
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

            if(vg == VoiceGroup.II && EditBufferModel.get().soundType.getValue() == SoundType.Split && key >= 60) {
                return (key * keyW) + (key * keyPadding);
            }
            return (key * keyW) + (key * keyPadding) + keyW + keyPadding;
        }

        private void drawAnfasser(Context2d ctx, boolean focus, Rect r, RGB stroke, RGBA fill) {
            r.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 2, 1, stroke, RGB.black());

            if (focus) {
                r.getReducedBy(-4).drawRoundedRect(ctx, Rect.ROUNDING_ALL, 1, 1, null, RGB.black());
                r.getReducedBy(-6).drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 3, null, stroke);
                r.getReducedBy(-7).drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 1, null, RGB.black());
            }
        }

        private void drawSplitAnfasser(Context2d ctx, VoiceGroup vg) {
            Rect anfasser = getSplitPointRect(vg);
            SelectedAnfasser focus = vg == VoiceGroup.I ? SelectedAnfasser.SplitPointI : SelectedAnfasser.SplitPointII;
            drawAnfasser(ctx, selection == focus, anfasser, presenter.getStrokeColor(vg), presenter.getFillColor(vg));
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

        public void drawSplit(Context2d ctx, int invalidationMask) {
            VoiceGroup vg = EditBufferModel.get().voiceGroup.getValue();

            if (vg == VoiceGroup.I) {
                drawSplitPart(ctx, VoiceGroup.II);
                drawSplitPart(ctx, VoiceGroup.I);

                drawSplitAnfasser(ctx, VoiceGroup.II);
                drawSplitAnfasser(ctx, VoiceGroup.I);
            } else {
                drawSplitPart(ctx, VoiceGroup.I);
                drawSplitPart(ctx, VoiceGroup.II);

                drawSplitAnfasser(ctx, VoiceGroup.I);
                drawSplitAnfasser(ctx, VoiceGroup.II);
            }
        }

        public double getFadePointIndicatorX(VoiceGroup vg) {
            Rect pix = getPixRect();
            return pix.getLeft() + quantizeToNoteBorder(presenter.getFadePointRange(vg).indicator, vg);
        }

        public double getFadeRangeX(VoiceGroup vg) {
            Rect pix = getPixRect();
            if (vg == VoiceGroup.I) {
                return Math.min(pix.getLeft() + quantizeToNoteBorder(getFadeFrom(VoiceGroup.I), vg)
                        + pix.getWidth() * getFadeRange(VoiceGroup.I), pix.getRight());
            } else {
                return Math.max(pix.getRight() - quantizeToNoteBorder(1.0 - getFadeFrom(VoiceGroup.II), vg)
                        - pix.getWidth() * getFadeRange(VoiceGroup.II), pix.getLeft());
            }
        }

        public Rect getLayerFadePointRect(VoiceGroup vg) {
            double size = 20;
            double halfSize = size / 2;

            Rect pix = getPixRect();
            if (vg == VoiceGroup.I) {
                return new Rect(getFadePointIndicatorX(VoiceGroup.I) - halfSize, pix.getTop() - halfSize, size, size);
            } else {
                return new Rect(getFadePointIndicatorX(VoiceGroup.II) - halfSize, pix.getTop() - halfSize, size, size);
            }
        }

        public Rect getSplitPointRect(VoiceGroup vg) {
            double size = 20;
            double halfSize = size / 2;
            Rect pix = getPixRect();
            double x = pix.getLeft() + getXPosForNote(presenter.getSplitRange(vg).indicator);
            return new Rect(x - halfSize, pix.getCenterPoint().getY() - halfSize, size, size);
        }

        public Rect getLayerFadeRangeRect(VoiceGroup vg) {
            double size = 20;
            double halfSize = size / 2;

            Rect pix = getPixRect();
            if (vg == VoiceGroup.I) {
                return new Rect(getFadeRangeX(VoiceGroup.I) - halfSize, pix.getBottom() - halfSize, size, size);
            } else {
                return new Rect(getFadeRangeX(VoiceGroup.II) - halfSize, pix.getBottom() - halfSize, size, size);
            }
        }

        private void drawFadeRangeAnfasser(Context2d ctx, VoiceGroup vg, RGB stroke, RGBA fill) {
            Rect anfasser = getLayerFadeRangeRect(vg);
            SelectedAnfasser focus = vg == VoiceGroup.I ? SelectedAnfasser.FadeRangeI : SelectedAnfasser.FadeRangeII;
            drawAnfasser(ctx, selection == focus, anfasser, stroke, fill);
        }

        private void drawFadePointAnfasser(Context2d ctx, VoiceGroup vg, RGB stroke, RGBA fill) {
            Rect anfasser = getLayerFadePointRect(vg);
            SelectedAnfasser focus = vg == VoiceGroup.I ? SelectedAnfasser.FadePointI : SelectedAnfasser.FadePointII;
            drawAnfasser(ctx, selection == focus, anfasser, stroke, fill);
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
            
            if(vg == VoiceGroup.I) {
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

        public void drawLayer(Context2d ctx, int invalidationMask) {

            RGB cI = new RGB(0x26, 0xb0, 0xff);
            RGBA cIFill = new RGBA(cI, 0.5);
            RGB cII = new RGB(0xff, 0x99, 0x33);
            RGBA cIIFill = new RGBA(cII, 0.5);

            if (EditBufferModel.get().voiceGroup.getValue() == VoiceGroup.I) {
                drawLayerPart(ctx, VoiceGroup.II);
                drawLayerPart(ctx, VoiceGroup.I);

                drawFadePointAnfasser(ctx, VoiceGroup.II, cII, cIIFill);
                drawFadeRangeAnfasser(ctx, VoiceGroup.II, cII, cIIFill);
                drawFadePointAnfasser(ctx, VoiceGroup.I, cI, cIFill);
                drawFadeRangeAnfasser(ctx, VoiceGroup.I, cI, cIFill);
            } else {
                drawLayerPart(ctx, VoiceGroup.I);
                drawLayerPart(ctx, VoiceGroup.II);

                drawFadePointAnfasser(ctx, VoiceGroup.I, cI, cIFill);
                drawFadeRangeAnfasser(ctx, VoiceGroup.I, cI, cIFill);
                drawFadePointAnfasser(ctx, VoiceGroup.II, cII, cIIFill);
                drawFadeRangeAnfasser(ctx, VoiceGroup.II, cII, cIIFill);
            }
        }

        private double getFadeFrom(VoiceGroup vg) {
            double v = EditBufferModel.get().getParameter(new ParameterId(396, vg)).value.value.getValue();
            return Math.min(1.0, Math.max(v, 0));
        }

        private double getFadeRange(VoiceGroup vg) {
            double v = EditBufferModel.get().getParameter(new ParameterId(397, vg)).value.value.getValue();
            return Math.min(1.0, Math.max(v, 0));
        }

        @Override
        public Control drag(Position pos, DragProxy dragProxy) {
            return super.drag(pos, dragProxy);
        }

        void selectControl(SelectedAnfasser anfasser) {
            if(anfasser == SelectedAnfasser.FadePointI || anfasser == SelectedAnfasser.FadeRangeI || anfasser == SelectedAnfasser.SplitPointI) {
                EditBufferUseCases.get().selectVoiceGroup(VoiceGroup.I);
            } else if(anfasser == SelectedAnfasser.FadePointII || anfasser == SelectedAnfasser.FadeRangeII || anfasser == SelectedAnfasser.SplitPointII) {
                EditBufferUseCases.get().selectVoiceGroup(VoiceGroup.II);
            }
            selection = anfasser;
            invalidate(INVALIDATION_FLAG_UI_CHANGED);
        }

        Control handleMouseDownDragStart(Position pos) {
            VoiceGroup first = EditBufferModel.get().voiceGroup.getValue();
            VoiceGroup other = first == VoiceGroup.I ? VoiceGroup.II : VoiceGroup.I;
            VoiceGroup vgs[] = { first, other };

            SoundType type = EditBufferModel.get().soundType.getValue();

            for (VoiceGroup vg : vgs) {
                if (type == SoundType.Layer) {
                    if (getLayerFadePointRect(vg).contains(pos)) {
                        selectControl(vg == VoiceGroup.I ? SelectedAnfasser.FadePointI : SelectedAnfasser.FadePointII);
                        return this;
                    }

                    if (getLayerFadeRangeRect(vg).contains(pos)) {
                        selectControl(vg == VoiceGroup.I ? SelectedAnfasser.FadeRangeI : SelectedAnfasser.FadeRangeII);
                        return this;
                    }
                } else {
                    if (getSplitPointRect(vg).contains(pos)) {
                        selectControl(
                                vg == VoiceGroup.I ? SelectedAnfasser.SplitPointI : SelectedAnfasser.SplitPointII);
                        return this;
                    }
                }
            }
            return null;
        }

        @Override
        public Control startDragging(Position pos) {
            Control c = handleMouseDownDragStart(pos);
            if (c == null) {
                return super.startDragging(pos);
            }
            return c;
        }

        @Override
        public Control click(Position pos) {
            Control c = handleMouseDownDragStart(pos);
            if (c == null) {
                return super.click(pos);
            }
            return c;
        }

        public double getCPForPosition(Position p) throws Exception {
            Rect pix = getPixRect();
            double xPercent = (p.getX() - pix.getLeft()) / pix.getWidth();

            double fadeI = EditBufferModel.get().getParameter(new ParameterId(396, VoiceGroup.I)).value.value
                    .getValue();
            double fadeII = EditBufferModel.get().getParameter(new ParameterId(396, VoiceGroup.II)).value.value
                    .getValue();

            switch (selection) {
                case FadePointI:
                case FadePointII:
                case SplitPointI:
                case SplitPointII:
                    return xPercent;
                // calculate useable range! to left / right of fade point and determine how far
                // the mouse has traveled -> new CP
                case FadeRangeI: {
                    double useableRange = Math.max(0, Math.min(pix.getWidth() - (pix.getWidth() * fadeI), pix.getWidth()));
                    double usableRangePercent = useableRange / pix.getWidth();
                    return Math.max(0,
                            Math.min((p.getX() - (pix.getLeft() + (pix.getWidth() * fadeI))) / pix.getWidth(),
                            usableRangePercent));
                }

                case FadeRangeII: {
                    double useableRange = Math.max(0, Math.min((pix.getWidth() * fadeII), pix.getWidth()));
                    double usableRangePercent = useableRange / pix.getWidth();
                    return Math.max(0,
                            Math.min(((pix.getLeft() + (pix.getWidth() * fadeII)) - p.getX()) / pix.getWidth(),
                                    usableRangePercent));
                }
                case None:
                default:
                    break;

            }

            throw new Exception("out of bounds!");
        }

        public void updateCP(Position p) {
            try {
                double cp = getCPForPosition(p);

                switch (selection) {
                    case FadePointI:
                        EditBufferUseCases.get().setParameterValue(new ParameterId(396, VoiceGroup.I), cp, false);
                        break;
                    case FadePointII:
                        EditBufferUseCases.get().setParameterValue(new ParameterId(396, VoiceGroup.II), cp, false);
                        break;
                    case FadeRangeI:
                        EditBufferUseCases.get().setParameterValue(new ParameterId(397, VoiceGroup.I), cp, false);
                        break;
                    case FadeRangeII:
                        EditBufferUseCases.get().setParameterValue(new ParameterId(397, VoiceGroup.II), cp, false);
                        break;
                    case SplitPointI:
                        EditBufferUseCases.get().setParameterValue(new ParameterId(356, VoiceGroup.I), cp, false);
                        break;
                    case SplitPointII:
                        EditBufferUseCases.get().setParameterValue(new ParameterId(356, VoiceGroup.II), cp, false);
                        break;
                    case None:
                    default:
                        break;
                }
            } catch (Exception e) {
                // expected!
            }
        }

        @Override
        public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
            if (selection != SelectedAnfasser.None) {
                updateCP(newPoint);
                return this;
            }
            return super.mouseDrag(oldPoint, newPoint, fine);
        }
    }

    private class PartFadeToggle extends SVGImage {

        public PartFadeToggle(Control parent) {
            super(parent, "Fade-Edit-Btn_Active.svg");
        }

        @Override
        public int getSelectedPhase() {
            return 0;
        }

        @Override
        public Control click(Position p) {
            NonMaps.get().getNonLinearWorld().getViewport().getOverlay().getBelt().openTab(BeltTab.Sound);
            return this;
        }
    }

    private class ValueDisplay extends Label {

        protected ParameterPresenter presenter;

        ValueDisplay(OverlayLayout parent, ParameterId parameterID) {
            super(parent);

            ParameterPresenterProviders.get().register(parameterID, p -> {
                presenter = p;
                return true;
            });
        }

        protected boolean isBiPolar() {
            return presenter.bipolar;
        }

        public double getValue() {
            return presenter.controlPosition;
        }

        public String getDisplayValue() {
            return presenter.displayValues[presenter.displayValues.length - 1];
        }

        @Override
        public String getDrawText(Context2d ctx) {
            return getDisplayValue();
        }
    }

    private class OctaveLabel extends Label {
        private String text;

        OctaveLabel(OverlayLayout p, int index) {
            super(p);
            text = "C" + (index + 1);
            setFontHeightInMM(5);
        }

        @Override
        public String getDrawText(Context2d ctx) {
            return text;
        }
    }

    private KeyBed keys;
    private OctaveLabel[] octaveLabels = new OctaveLabel[6];
    private PartFadeToggle toggle;
    private ValueDisplay fadePointI;
    private ValueDisplay fadePointII;

    protected BeltFadeEditorLayout(Control parent) {
        super(parent);

        keys = addChild(new KeyBed(this));
        toggle = addChild(new PartFadeToggle(this));
        fadePointI = addChild(new ValueDisplay(this, new ParameterId(396, VoiceGroup.I)));
        fadePointII = addChild(new ValueDisplay(this, new ParameterId(396, VoiceGroup.II)));

        for (int i = 0; i < 6; i++) {
            octaveLabels[i] = addChild(new OctaveLabel(this, i));
        }

        EditBufferModel.get().soundType.onChange(type -> {
            installType(type);
            return true;
        });

    }

    private void installType(SoundType type) {
        if (type == SoundType.Split) {

        } else if (type == SoundType.Layer) {

        }
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        super.doLayout(x, y, w, h);
        double parts = 10;
        double partWidth = w / parts;
        double keysW = partWidth * 6;
        double keysLeft = x + partWidth * 2;
        keys.doLayout(x + (w - keys.getPictureWidth()) / 2, y + (keys.getPictureHeight() / 2), keys.getPictureWidth(),
                keys.getPictureHeight());

        for (int i = 0; i < 6; i++) {
            octaveLabels[i].doLayout(keys.getPixRect().getLeft() + keys.quantizeToNoteBorder(i * 12.0 * (1.0 / 61.0), VoiceGroup.Global) - 30,
                    keys.getPictureHeight() * 1.6, 50, 40);
        }

        double valueHeigth = h / 4;
        fadePointI.doLayout(keysLeft - partWidth, y + valueHeigth * 1.5, partWidth, valueHeigth);
        fadePointII.doLayout(keysLeft + keysW - partWidth, y + valueHeigth * 1.5, partWidth, valueHeigth);

        double pw = toggle.getPictureWidth();
        double ph = toggle.getPictureHeight();
        toggle.doLayout(w - partWidth, h / 2 - (ph / 2), pw, ph);
    }

    @Override
    public Control startDragging(Position pos) {
        Control c = keys.startDragging(pos);
        if (c == null)
            return super.startDragging(pos);
        return c;
    }

    @Override
    public Control click(Position pos) {
        Control c = keys.click(pos);
        if (c == null)
            return super.click(pos);
        return c;
    }

    @Override
    public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
        Control c = keys.mouseDrag(oldPoint, newPoint, fine);
        if (c == null)
            return super.mouseDrag(oldPoint, newPoint, fine);
        return c;
    }
}
