package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class SplitFXIndicator extends SVGImage {
    
    private VoiceGroup voiceGroup = VoiceGroup.I;

    public SplitFXIndicator(VoiceGroup vg, OverlayLayout parent) {
        super(parent, vg == VoiceGroup.I ? "Split_FX_B.svg" : "Split_FX_A.svg");
        voiceGroup = vg;

        EditBufferPresenterProvider.get().onChange(ebp -> {
            if(voiceGroup == VoiceGroup.I) {
                setVisible(ebp.splitFXToII);
            } else if(voiceGroup == VoiceGroup.II) {
                setVisible(ebp.splitFXToI);
            }
            return true;
        });
    }

    @Override
    public int getSelectedPhase() {
        return 0;
    }
}