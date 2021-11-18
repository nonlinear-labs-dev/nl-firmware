package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class LayerSoundFBIndicator extends SVGImage {
    
    private int selectedPhase = 0;

    public LayerSoundFBIndicator(OverlayLayout parent) {
        super(parent, "Layer_FB_A.svg", "Layer_FB_B.svg", "Layer_FB_C.svg");

        EditBufferPresenterProvider.get().onChange(ebp -> {

            if(ebp.layerFBI && ebp.layerFBII) {
                selectedPhase = 2;
            } else if(ebp.layerFBI && !ebp.layerFBII) {
                selectedPhase = 0;
            } else if(!ebp.layerFBI && ebp.layerFBII) {
                selectedPhase = 1;
            }
            
            setVisible(ebp.layerFBI || ebp.layerFBII);

            return true;
        });
    }

    @Override
    public int getSelectedPhase() {
        return selectedPhase;
    }
}