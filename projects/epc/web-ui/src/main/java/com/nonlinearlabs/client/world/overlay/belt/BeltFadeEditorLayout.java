package com.nonlinearlabs.client.world.overlay.belt;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.ValueLabel;
import com.nonlinearlabs.client.Millimeter;
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
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.world.overlay.belt.Belt.BeltTab;
import com.nonlinearlabs.client.world.overlay.belt.fadeeditor.KeyBed;
import com.nonlinearlabs.client.world.overlay.belt.parameters.ValueDisplay;
import com.nonlinearlabs.client.presenters.*;
import com.nonlinearlabs.client.presenters.FadeEditorPresenter.KeyRange;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class BeltFadeEditorLayout extends OverlayLayout {

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

    private abstract class ValueDisplay extends Label {

        private VoiceGroup voiceGroup;

        ValueDisplay(OverlayLayout parent, VoiceGroup vg) {
            super(parent);
            voiceGroup = vg;
            setFontHeightInMM(5);
        }

        @Override
        public RGB getColorFont() {
            return presenter.getStrokeColor(voiceGroup);
        }

        @Override
        public void draw(Context2d ctx, int flags) {
            getPixRect().drawValueEditSliderBackgound(ctx, false, getColorFont());
            super.draw(ctx, flags);
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

    private class LayerPartIndicators extends OverlayLayout {

        private VoiceGroup voiceGroup;

        private ValueDisplay fadePoint;
        private ValueDisplay fadeRange;

        LayerPartIndicators(OverlayLayout p, VoiceGroup vg) {
            super(p);
            voiceGroup = vg;

            fadePoint = addChild(new ValueDisplay(this, vg) {
                @Override
                public String getDrawText(Context2d ctx) {
                    return presenter.getFadePointText(voiceGroup);
                }
            });

            fadeRange = addChild(new ValueDisplay(this, vg) {
                @Override
                public String getDrawText(Context2d ctx) {
                    return presenter.getFadeRangeText(voiceGroup);
                }

                @Override
                protected Rect getTextRect() {
                    Rect r = super.getTextRect().copy();
                    r.reduceWidthBy(Millimeter.toPixels(5));
                    return r;
                }
            });
        }

        @Override
        public void doLayout(double x, double y, double w, double h) {
            super.doLayout(x, y, w, h);

            double pH = h / 4;
            fadePoint.doLayout(0, 0, w, pH);
            fadeRange.doLayout(0, pH / 2 * 3, w, pH);
        }
    }

    private class SplitPartIndicators extends OverlayLayout {
        private VoiceGroup voiceGroup;

        private ValueDisplay split;

        SplitPartIndicators(OverlayLayout p, VoiceGroup vg) {
            super(p);
            voiceGroup = vg;
            split = addChild(new ValueDisplay(this, vg) {
                @Override
                public String getDrawText(Context2d ctx) {
                    return presenter.getSplitPointText(voiceGroup);
                }
            });
        }

        @Override
        public void doLayout(double x, double y, double w, double h) {
            super.doLayout(x, y, w, h);

            double pH = h / 4;
            split.doLayout(0, pH / 2, w, pH);
        }
    }

    private KeyBed keys;
    private OctaveLabel[] octaveLabels = new OctaveLabel[6];
    private PartFadeToggle toggle;

    private OverlayControl leftControls = null;
    private OverlayControl rightControls = null;

    private FadeEditorPresenter presenter;

    protected BeltFadeEditorLayout(Control parent) {
        super(parent);

        presenter = FadeEditorPresenterProvider.get().getValue();

        FadeEditorPresenterProvider.get().onChange(p -> {
            presenter = p;
            return true;
        });

        keys = addChild(new KeyBed(this));
        toggle = addChild(new PartFadeToggle(this));

        for (int i = 0; i < 6; i++) {
            octaveLabels[i] = addChild(new OctaveLabel(this, i));
        }

        EditBufferModel.get().soundType.onChange(type -> {
            installType(type);
            return true;
        });
    }

    private void installType(SoundType type) {

        if (leftControls != null || rightControls != null) {
            removeChild(leftControls);
            removeChild(rightControls);
            leftControls = null;
            rightControls = null;
        }

        if (type == SoundType.Split) {
            leftControls = addChild(new SplitPartIndicators(this, VoiceGroup.I));
            rightControls = addChild(new SplitPartIndicators(this, VoiceGroup.II));
        } else if (type == SoundType.Layer) {
            leftControls = addChild(new LayerPartIndicators(this, VoiceGroup.I));
            rightControls = addChild(new LayerPartIndicators(this, VoiceGroup.II));
        }

        requestLayout();
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        super.doLayout(x, y, w, h);

        double parts = 15;
        double partWidth = w / parts;
        double baseLine = y + (keys.getPictureHeight() / 2);
        keys.doLayout((w - keys.getPictureWidth()) / 2, (keys.getPictureHeight() / 2), keys.getPictureWidth(),
                keys.getPictureHeight());

        for (int i = 0; i < 6; i++) {
            octaveLabels[i].doLayout(
                    keys.getRelativePosition().getLeft()
                            + keys.quantizeToNoteBorder(i * 12.0 * (1.0 / 61.0), VoiceGroup.Global) - 30,
                    keys.getPictureHeight() * 1.6, 50, 40);
        }

        double pw = toggle.getPictureWidth();
        double ph = toggle.getPictureHeight();
        toggle.doLayout(keys.getRelativePosition().getRight() + partWidth * 2, h / 2 - (ph / 2), pw, ph);

        if (leftControls != null)
            leftControls.doLayout(keys.getRelativePosition().getLeft() - partWidth * 1.5, baseLine, partWidth, h);

        if (rightControls != null)
            rightControls.doLayout(keys.getRelativePosition().getRight() + partWidth * 0.5, baseLine, partWidth, h);
    }

    // Handle overlapping Anfasser inside KeyBed
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
