package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.presenters.FadeEditorPresenter;
import com.nonlinearlabs.client.presenters.FadeEditorPresenterProvider;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;
import com.nonlinearlabs.client.world.overlay.belt.Belt.BeltTab;

class KeyBedEditor extends OverlayLayout {
    protected FadeEditorPresenter presenter;
    protected OctaveLabel[] octaveLabels = new OctaveLabel[6];
    protected CloseButton closeButton;

    protected class CloseButton extends SVGImage {

        public CloseButton(Control parent) {
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

    protected class ValueDisplay extends ValueEdit {

        private VoiceGroup voiceGroup;

        ValueDisplay(OverlayLayout parent, ParameterId id) {
            super(parent, id, false);
            voiceGroup = id.getVoiceGroup();
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

    protected class OctaveLabel extends Label {
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

    protected KeyBedEditor(Control parent) {
        super(parent);

        closeButton = addChild(new CloseButton(this));

        for (int i = 0; i < 6; i++)
            octaveLabels[i] = addChild(new OctaveLabel(this, i));

        FadeEditorPresenterProvider.get().onChange(p -> {
            presenter = p;
            return true;
        });

    }

}