package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.belt.fadeeditor.KeyBed;
import com.nonlinearlabs.client.world.overlay.belt.fadeeditor.SplitPointKeyBed;
import com.nonlinearlabs.client.world.overlay.belt.parameters.SyncParameterButton;

public class SplitPointsKeyBedEditor extends KeyBedEditor {

    private SyncParameterButton button;
    private OverlayControl leftControls = null;
    private OverlayControl rightControls = null;
    private KeyBed keys;
    private HorizonatlScrollPane keysPane;

    protected SplitPointsKeyBedEditor(Control parent) {
        super(parent);

        keysPane = addChild(new HorizonatlScrollPane(this, keys = new SplitPointKeyBed(this)));
        button = addChild(new SyncParameterButton(this));
        leftControls = addChild(new ValueDisplay(this, new ParameterId(356, VoiceGroup.I)));
        rightControls = addChild(new ValueDisplay(this, new ParameterId(356, VoiceGroup.II)));
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        super.doLayout(x, y, w, h);

        int parts = 8;
        double partWidth = w / parts;

        // button and close button have to be in sync with buttons in SoundLayout
        double settingsWidth = closeButton.getPictureWidth();
        double settingsHeight = closeButton.getPictureHeight();
        double xMargin = settingsWidth * 0.5;
        button.doLayout(7 * partWidth - settingsWidth / 2, h / 2 - settingsHeight / 2, settingsWidth, settingsHeight);
        closeButton.doLayout(button.getRelativePosition().getRight() + xMargin, h / 2 - settingsHeight / 2,
                settingsWidth, settingsHeight);

        // right control aligns itself to the buttons
        double valueWidth = Math.max(partWidth * 0.75, Millimeter.toPixels(10));
        rightControls.doLayout(button.getRelativePosition().getLeft() - valueWidth - xMargin, h / 2 - h / 8, valueWidth,
                h / 4);

        // left button should be same distance to border as right button
        leftControls.doLayout(w - rightControls.getRelativePosition().getRight(), h / 2 - h / 8, valueWidth, h / 4);

        // the rest is for the keys
        double keysLeft = leftControls.getRelativePosition().getRight() + xMargin;
        double keysRight = rightControls.getRelativePosition().getLeft() - xMargin;
        double keysWidth = Math.max(keysRight - keysLeft, 0);
        double keysWidthClamped = Math.min(keys.getPictureWidth(), keysWidth);
        keysLeft += (keysWidth - keysWidthClamped) / 2;

        keysPane.doLayout(keysLeft, 0, keysWidthClamped, h);
    }
}
