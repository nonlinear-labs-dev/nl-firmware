package com.nonlinearlabs.client.world.overlay.belt;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class BeltFadeEditorLayout extends OverlayLayout {

    private class KeyBed extends SVGImage {
        public KeyBed(Control parent) {
            super(parent, "Keys-Fade-Range.svg");
        }

        @Override
        public int getSelectedPhase() {
            return 0;
        }
    }

    private KeyBed keys;

    protected BeltFadeEditorLayout(Control parent) {
        super(parent);

        keys = addChild(new KeyBed(this));

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
        keys.doLayout(x + partWidth * 2, y, keysW, h);
    }
}
