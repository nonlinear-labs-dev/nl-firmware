package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.fadeeditor.FadePointKeyBed;
import com.nonlinearlabs.client.world.overlay.belt.fadeeditor.KeyBed;

public class FadePointsKeyBedEditor extends KeyBedEditor {

    private class LayerPartIndicators extends OverlayLayout {

        private VoiceGroup voiceGroup;
        private ValueDisplay fadePoint;
        private ValueDisplay fadeRange;

        LayerPartIndicators(OverlayLayout p, VoiceGroup vg) {
            super(p);
            voiceGroup = vg;

            fadePoint = addChild(new ValueDisplay(this, new ParameterId(396, voiceGroup)));
            fadeRange = addChild(new ValueDisplay(this, new ParameterId(397, voiceGroup)));
        }

        @Override
        public void doLayout(double x, double y, double w, double h) {
            super.doLayout(x, y, w, h);

            double pH = h / 4;
            fadePoint.doLayout(0, 0, w, pH);
            fadeRange.doLayout(0, pH / 2 * 3, w, pH);
        }
    }

    private OverlayControl leftControls = null;
    private OverlayControl rightControls = null;
    private KeyBed keys;

    protected FadePointsKeyBedEditor(Control parent) {
        super(parent);

        keys = addChild(new FadePointKeyBed(this));
        leftControls = addChild(new LayerPartIndicators(this, VoiceGroup.I));
        rightControls = addChild(new LayerPartIndicators(this, VoiceGroup.II));
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        super.doLayout(x, y, w, h);

        int parts = 8;
        double partWidth = w / parts;

        double baseLine = y + (keys.getPictureHeight() / 2);
        keys.doLayout((w - keys.getPictureWidth()) / 2, (keys.getPictureHeight() / 2), keys.getPictureWidth(),
                keys.getPictureHeight() + KeyBed.HANDLE_SIZE);

        for (int i = 0; i < 6; i++) {
            octaveLabels[i].doLayout(
                    keys.getRelativePosition().getLeft()
                            + keys.quantizeToNoteBorder(i * 12.0 * (1.0 / 61.0), VoiceGroup.Global) - 30,
                    keys.getPictureHeight() * 1.6, 50, 40);
        }

        double closeW = closeButton.getPictureWidth();
        double closeH = closeButton.getPictureHeight();
        closeButton.doLayout(7 * partWidth - closeW / 2 + closeW * 1.5, h / 2 - closeH / 2, closeW, closeH);

        double valueWidth = partWidth * 0.75;
        leftControls.doLayout(keys.getRelativePosition().getLeft() - valueWidth * 1.25, baseLine, valueWidth, h);
        rightControls.doLayout(keys.getRelativePosition().getRight() + valueWidth * 0.25, baseLine, valueWidth, h);
    }

    // Handle overlapping Handles inside KeyBed
    @Override
    public Control startDragging(Position pos) {
        Control c = keys.startDragging(pos);
        if (c == null)
            return super.startDragging(pos);
        return c;
    }

    @Override
    public Control click(Position pos) {
        Control c = keys.click(pos);
        if (c == null)
            return super.click(pos);
        return c;
    }

    @Override
    public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
        Control c = keys.mouseDrag(oldPoint, newPoint, fine);
        if (c == null)
            return super.mouseDrag(oldPoint, newPoint, fine);
        return c;
    }
}
