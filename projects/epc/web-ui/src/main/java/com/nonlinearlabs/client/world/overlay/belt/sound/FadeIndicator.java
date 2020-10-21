package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.core.client.GWT;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.overlay.SVGImage;


public class FadeIndicator extends SVGImage {

    private VoiceGroup voiceGroup;

    private boolean active = false;

    public FadeIndicator(Control parent, VoiceGroup vg) {
        super(parent, "Part-Fade-Ind_A.svg", "Part_Fade-Ind_B.svg", "Part_Fade-Ind_C.svg");
        voiceGroup = vg;

        ParameterPresenterProviders.get().register(new ParameterId(396, vg), presenter -> {
            active = !presenter.isDefault;
            return true;
        });
    }

    @Override
    public int getSelectedPhase() {
        if(!active) {
            return 0;
        }

        if(voiceGroup == VoiceGroup.I) {
            return 1;
        } else {
            return 2;
        }
    }
    
}
