package com.nonlinearlabs.client.world.overlay.belt;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.world.overlay.belt.Belt.BeltTab;
import com.nonlinearlabs.client.presenters.*;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class BeltFadeEditorLayout extends OverlayLayout {

    private class Anfasser extends OverlayControl {
        
        VoiceGroup voiceGroup;
        Anfasser(OverlayControl parent, VoiceGroup vg) {
            super(parent);
            voiceGroup = vg;
        }
        @Override
        public void draw(Context2d ctx, int invalidationMask) {
            RGB color = voiceGroup == VoiceGroup.I ? new RGB(0x26, 0xb0, 0xff) : new RGB(0xff, 0x99, 0x33);
            Rect p = getPixRect();
            p.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 1, color.withAlpha(0.4), color);
        }
    }

    private class KeyBed extends SVGImage {
        public KeyBed(Control parent) {
            super(parent, "Keys-Fade-Range.svg");
            
        }

        @Override
        public int getSelectedPhase() {
            return 0;
        }

        @Override
        public void draw(Context2d ctx, int invalidationMask) {
            super.draw(ctx, invalidationMask);
            if(EditBufferModel.get().soundType.getValue() == SoundType.Layer) {
                drawLayer(ctx, invalidationMask);
            } else if(EditBufferModel.get().soundType.getValue() == SoundType.Split) {
                drawSplit(ctx, invalidationMask);
            }
        }

        public double getDrawPositon(double splitValue) {
            int totalKeys = 61;
            
            int keyW = 13;
            int keyPadding = 1;

            int key = (int) (splitValue * totalKeys);

            if((splitValue == 0 || splitValue == 1) && EditBufferModel.get().soundType.getValue() == SoundType.Layer) {
                return (key * keyW) + (key * keyPadding);
            } else {
                return (key * keyW) + (key * keyPadding) + keyW + keyPadding;
            }
        }

        public void drawSplitPartI(Context2d ctx, RGB stroke, RGBA fill) {
            Rect pix = getPixRect().copy();
            double splitI = getDrawPositon(getSplitPoint(VoiceGroup.I));

            ctx.beginPath();
            ctx.setFillStyle(fill.toString());
            ctx.setStrokeStyle(stroke.toString());
            ctx.moveTo(pix.getLeft(), pix.getTop());
            ctx.lineTo(pix.getLeft() + splitI, pix.getTop());
            ctx.lineTo(pix.getLeft() + splitI, pix.getBottom());
            ctx.lineTo(pix.getLeft(), pix.getBottom());
            ctx.lineTo(pix.getLeft(), pix.getTop());
            ctx.fill();
            ctx.stroke();
        }

        public void drawSplitPartII(Context2d ctx, RGB stroke, RGBA fill) {
            Rect pix = getPixRect().copy();
            double splitII = getDrawPositon(1.0 - getSplitPoint(VoiceGroup.II));

            ctx.beginPath();
            ctx.setFillStyle(fill.toString());
            ctx.setStrokeStyle(stroke.toString());
            ctx.moveTo(pix.getRight(), pix.getTop());
            ctx.lineTo(pix.getRight(), pix.getBottom());
            ctx.lineTo(pix.getRight() - splitII, pix.getBottom());
            ctx.lineTo(pix.getRight() - splitII, pix.getTop());
            ctx.lineTo(pix.getRight(), pix.getTop());
            ctx.fill();
            ctx.stroke();
        }

        public Rect getSplitAnfasser(VoiceGroup vg) {
            Rect pix = getPixRect();
            double v = getSplitPoint(vg);
            if(vg == VoiceGroup.II)
             v = 1 - v;
            double start = vg == VoiceGroup.I ? pix.getLeft() : pix.getRight();

            return new Rect(start + v - 5, pix.getCenterPoint().getY(), 10, 10);
        }

        public void drawSplit(Context2d ctx, int invalidationMask) {

            RGB cI = new RGB(0x26, 0xb0, 0xff);
            RGBA cIFill = new RGBA(cI, 0.5);
            RGB cII = new RGB(0xff, 0x99, 0x33); 
            RGBA cIIFill = new RGBA(cII, 0.5); 

            Rect anfasserI = getSplitAnfasser(VoiceGroup.I);
            Rect anfasserII = getSplitAnfasser(VoiceGroup.II);

            VoiceGroup vg = EditBufferModel.get().voiceGroup.getValue();

            Rect selectedAnfasserRect; 

            if(vg == VoiceGroup.I) {
                drawSplitPartI(ctx, cI, cIFill);
                drawSplitPartII(ctx, cII, cIIFill);
                anfasserII.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 1, cII, RGB.black());
                anfasserI.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 1, cI, RGB.black());
                selectedAnfasserRect = anfasserI.copy().getReducedBy(-5);
                selectedAnfasserRect.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 2, null, cI);
            } else {
                drawSplitPartII(ctx, cII, cIIFill);
                drawSplitPartI(ctx, cI, cIFill);
                anfasserI.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 1, cI, RGB.black());
                anfasserII.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 1, cII, RGB.black());    
                selectedAnfasserRect = anfasserII.copy().getReducedBy(-5);
                selectedAnfasserRect.drawRoundedRect(ctx, Rect.ROUNDING_ALL, 3, 2, null, cII);
            }
        
        }

        public void drawLayer(Context2d ctx, int invalidationMask) {
            
            RGB cI = new RGB(0x26, 0xb0, 0xff);
            RGBA cIFill = new RGBA(cI, 0.5);
            RGB cII = new RGB(0xff, 0x99, 0x33); 
            RGBA cIIFill = new RGBA(cII, 0.5); 
            
            Rect pix = getPixRect().copy();
            
            double fadeFromI = getDrawPositon(getFadeFrom(VoiceGroup.I)); 
            double fadeRangeI = pix.getWidth() * getFadeRange(VoiceGroup.I);

            double fadeFromII = getDrawPositon(1.0 - getFadeFrom(VoiceGroup.II)); 
            double fadeRangeII = pix.getWidth() * getFadeRange(VoiceGroup.II);

            ctx.beginPath();
            ctx.setFillStyle(cIFill.toString());
            ctx.setStrokeStyle(cI.toString());
            ctx.moveTo(pix.getLeft(), pix.getTop());
            ctx.lineTo(pix.getLeft() + Math.max(fadeFromI, 0), pix.getTop());
            double right = Math.min(pix.getLeft() + fadeFromI + fadeRangeI, pix.getRight());
            ctx.lineTo(right, pix.getBottom());
            ctx.lineTo(pix.getLeft(), pix.getBottom());
            ctx.lineTo(pix.getLeft(), pix.getTop());
            ctx.fill();
            ctx.stroke();

            ctx.beginPath();
            ctx.setFillStyle(cIIFill.toString());
            ctx.setStrokeStyle(cII.toString());
            ctx.moveTo(pix.getRight(), pix.getTop());
            ctx.lineTo(pix.getRight() - Math.max(fadeFromII, 0), pix.getTop());
            double left = Math.max(pix.getRight() - fadeFromII - fadeRangeII, pix.getLeft());
            ctx.lineTo(left, pix.getBottom());
            ctx.lineTo(pix.getRight(), pix.getBottom());
            ctx.lineTo(pix.getRight(), pix.getTop());
            ctx.fill();
            ctx.stroke();
        }

        private double getFadeFrom(VoiceGroup vg) {
            double v = EditBufferModel.get().getParameter(new ParameterId(396, vg)).value.value.getValue();
            return Math.min(1.0, Math.max(v, 0));
        }

        private double getFadeRange(VoiceGroup vg) {
            double v =  EditBufferModel.get().getParameter(new ParameterId(397, vg)).value.value.getValue();
            return Math.min(1.0, Math.max(v, 0));
        }

        private double getSplitPoint(VoiceGroup vg) {
            double v = EditBufferModel.get().getParameter(new ParameterId(356, vg)).value.value.getValue();
            return Math.min(1.0, Math.max(v, 0));
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
        

        for(int i = 0; i < 6; i++) {
            octaveLabels[i] = addChild(new OctaveLabel(this, i));
        }

        EditBufferModel.get().soundType.onChange(type -> {
            installType(type);       
            return true;
        });

    }

    private void installType(SoundType type) {
        if(type == SoundType.Split) {

        } else if(type == SoundType.Layer) {

        }
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        super.doLayout(x, y, w, h);
        double parts = 10;
        double partWidth = w / parts;
        double keysW = partWidth * 6;
        double keysLeft = x + partWidth * 2;
        keys.doLayout(x + (w - keys.getPictureWidth()) / 2, y + (keys.getPictureHeight() / 2), keys.getPictureWidth(), keys.getPictureHeight());
    
        for(int i = 0; i < 6; i++) {
            octaveLabels[i].doLayout(keys.getPixRect().getLeft() + keys.getDrawPositon(i * 12.0 * (1.0 / 61.0)) - 30, keys.getPictureHeight() * 1.6, 50, 40);
        }

        double valueHeigth = h / 4;
        fadePointI.doLayout(keysLeft - partWidth, y + valueHeigth * 1.5, partWidth, valueHeigth);
        fadePointII.doLayout(keysLeft + keysW  - partWidth, y + valueHeigth * 1.5, partWidth, valueHeigth);

        double pw = toggle.getPictureWidth();
        double ph = toggle.getPictureHeight();
        toggle.doLayout(w - partWidth, h / 2 - (ph / 2), pw, ph);
    }
}
