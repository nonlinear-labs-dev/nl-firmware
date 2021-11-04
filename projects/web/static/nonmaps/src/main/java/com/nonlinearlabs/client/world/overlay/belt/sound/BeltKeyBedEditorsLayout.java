package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.Belt;

public class BeltKeyBedEditorsLayout extends OverlayLayout {

    public BeltKeyBedEditorsLayout(Belt belt) {
        super(belt);

        EditBufferModel.get().soundType.onChange(type -> {
            removeAll();

            switch (type) {
                case Layer:
                    addChild(new FadePointsKeyBedEditor(this));
                    break;

                case Single:
                    break;

                case Split:
                    addChild(new SplitPointsKeyBedEditor(this));
                    break;
                default:
                    break;

            }
            return true;
        });
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        if (!getChildren().isEmpty())
            getChildren().get(0).doLayout(0, 0, w, h);
        super.doLayout(x, y, w, h);
    }

}