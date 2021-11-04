package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class LayerSoundFXIndicator extends SVGImage {
    
    private int selectedPhase = 0;

    public LayerSoundFXIndicator(OverlayLayout parent) {
        super(parent, "Layer_FX_A.svg", "Layer_FX_B.svg", "Layer_FX_C.svg");

        EditBufferPresenterProvider.get().onChange(ebp -> {

            if(ebp.layerFXToI && ebp.layerFXToII) {
                selectedPhase = 2;
            } else if(ebp.layerFXToI && !ebp.layerFXToII) {
                selectedPhase = 0;
            } else if(!ebp.layerFXToI && ebp.layerFXToII) {
                selectedPhase = 1;
            }
            
            setVisible(ebp.layerFXToI || ebp.layerFXToII);

            return true;
        });
    }

    @Override
    public int getSelectedPhase() {
        return selectedPhase;
    }
}