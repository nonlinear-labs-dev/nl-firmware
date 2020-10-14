package com.nonlinearlabs.client.presenters;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;

public class FadeEditorPresenter {

    public double splitI;
    public double splitII;

    public double fadePointI;
    public double fadePointII;
    public double fadeRangeI;
    public double fadeRangeII;

    public class KeyRange  {
        public int from;
        public int to;
        public int indicator;
    }

    public KeyRange splitRangeI = new KeyRange();
    public KeyRange splitRangeII = new KeyRange();

    public RGB colorVGI = new RGB(0x26, 0xb0, 0xff);
    public RGBA colorVGIFill = new RGBA(colorVGI, 0.5);
    public RGB colorVGII = new RGB(0xff, 0x99, 0x33);
    public RGBA colorVGIIFill = new RGBA(colorVGII, 0.5);

    enum SelectedFadeControl {
        FadePoint, FadeRange, Split
    }

    public SelectedFadeControl selectedControl;

    public double getSplitValue(VoiceGroup vg) {
        if(vg == VoiceGroup.I) {
            return splitI;
        } else {
            return splitII;
        }
    }

    public double getFadePointValue(VoiceGroup vg) {
        if(vg == VoiceGroup.I) {
            return fadePointI;
        } else {
            return fadePointII;
        }
    }

    public double getFadeRangeValue(VoiceGroup vg) {
        if(vg == VoiceGroup.I) {
            return fadeRangeI;
        } else {
            return fadeRangeII;
        }
    }

    public FadeEditorPresenter.KeyRange getSplitRange(VoiceGroup vg) {
        if(vg == VoiceGroup.I) {
            return splitRangeI;
        } else {
            return splitRangeII;
        }
    }

    public RGBA getStrokeColor(VoiceGroup vg) {
        if(vg == VoiceGroup.I) {
            return new RGBA(colorVGI, 1);
        } else {
            return new RGBA(colorVGII, 1);
        }
    }

    public RGBA getFillColor(VoiceGroup vg) {
        if(vg == VoiceGroup.I) {
            return colorVGIFill;
        } else {
            return colorVGIIFill;
        }
    }
}
