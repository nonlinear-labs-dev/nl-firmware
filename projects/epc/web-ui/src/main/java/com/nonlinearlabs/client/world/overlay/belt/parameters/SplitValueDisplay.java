package com.nonlinearlabs.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.overlay.Label;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;

public class SplitValueDisplay extends Label {

    public SplitValueDisplay(OverlayLayout parent) {
        super(parent);
        setFontHeightInMM(6);
    }

    @Override
    public String getDrawText(Context2d ctx) {
        final ParameterPresenter sI = ParameterPresenterProviders.get().getParameterPresenter(VoiceGroup.I, 356);
        final ParameterPresenter sII = ParameterPresenterProviders.get().getParameterPresenter(VoiceGroup.II, 356);
        char delim = hasOverlap() ? '-' : '|';

        if(getLargerPart() == VoiceGroup.I) {
            return sII.displayValues[0] + "\t " + delim + " \t" + sI.displayValues[0];
        } else {
            return sI.displayValues[0] + "\t " + delim + " \t" + sII.displayValues[0];
        }
    }

    // split 0 == VoiceGroup I
    // split 1 == '-'
    // split 2 == VoiceGroup II

    private boolean hasOverlap() {
        return getSplit(VoiceGroup.I).value.value.getValue() > getSplit(VoiceGroup.II).value.value.getValue();
    }

    private RGB getVoiceGroupColorFont(VoiceGroup vg) {
        VoiceGroup selectedVG = EditBufferModel.get().voiceGroup.getValue();
        if (vg == selectedVG) {
            return getColorFont();
        } else {
            return getColorFont().darker(50);
        }
    }

    private BasicParameterModel getSplit(VoiceGroup vg) {
        return EditBufferModel.get().getParameter(new ParameterId(356, vg));
    }

    private VoiceGroup getLargerPart() {
        return getSplit(VoiceGroup.I).value.value.getValue() > getSplit(VoiceGroup.II).value.value.getValue() ? VoiceGroup.I : VoiceGroup.II;
    }

    private VoiceGroup getSmallerPart() {
        return getSplit(VoiceGroup.I).value.value.getValue() < getSplit(VoiceGroup.II).value.value.getValue() ? VoiceGroup.I : VoiceGroup.II;
    }

    @Override
    protected RGB getColorFontForSplit(int i) {
        switch (i) {
            case 0:
                return getVoiceGroupColorFont(VoiceGroup.I);
            case 2:
                return getVoiceGroupColorFont(VoiceGroup.II);
            case 1:
            default:
                return getColorFont().darker(50);
        }
    }
}
