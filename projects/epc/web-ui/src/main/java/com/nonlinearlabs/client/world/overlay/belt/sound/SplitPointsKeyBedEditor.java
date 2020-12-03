package com.nonlinearlabs.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.contextStates.ClipContext;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Dimension;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayControl;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.belt.fadeeditor.KeyBed;
import com.nonlinearlabs.client.world.overlay.belt.fadeeditor.SplitPointKeyBed;
import com.nonlinearlabs.client.world.overlay.belt.parameters.SyncParameterButton;
import com.nonlinearlabs.client.world.pointer.Down;
import com.nonlinearlabs.client.world.pointer.Gesture;

public class SplitPointsKeyBedEditor extends KeyBedEditor {

    private SyncParameterButton button;
    private OverlayControl leftControls = null;
    private OverlayControl rightControls = null;
    private KeyBed keys;
    private HorizonatlScrollPane keysPane;

    class HorizonatlScrollPane extends OverlayLayout {
        double scrolling = 0;
        KeyBed nested;

        HorizonatlScrollPane(OverlayLayout parent, KeyBed nested) {
            super(parent);
            this.nested = addChild(nested);
        }

        @Override
        public void doLayout(double x, double y, double w, double h) {
            super.doLayout(x, y, w, h);
            getChildren().get(0).doLayout(0, 0, this.nested.getPictureWidth(), h);
            setScroll(scrolling);
        }

        void setScroll(double a) {
            double max = getRelativePosition().getWidth() - nested.getPictureWidth();
            a = Math.max(max, Math.min(0, a));

            if (a != scrolling) {
                scrolling = a;
                invalidate(INVALIDATION_FLAG_SCROLLED);
            }
        }

        @Override
        public void draw(Context2d ctx, int invalidationMask) {
            try (ClipContext c = new ClipContext(ctx, this)) {
                ctx.save();
                ctx.translate(scrolling, 0);
                super.draw(ctx, invalidationMask);
                ctx.restore();
            }
        }

        @Override
        public Control mouseDown(Position eventPoint) {
            return this;
        }

        @Override
        public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
            Dimension dim = newPoint.getVector(oldPoint);
            setScroll(scrolling + dim.getWidth());
            return this;
        }

        @Override
        public Control handleGesture(Gesture g) {
            if (g instanceof Down) {
                Down d = (Down) g;
                d.getPosition().moveBy(-scrolling, 0);
                Control ret = super.handleGesture(g);
                d.getPosition().moveBy(scrolling, 0);
                return ret;
            }
            return super.handleGesture(g);

        }
    }

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
        double valueWidth = partWidth * 0.75;
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

        keysPane.doLayout(keysLeft, (keys.getPictureHeight() / 2), keysWidthClamped,
                keys.getPictureHeight() + KeyBed.HANDLE_SIZE);

        for (int i = 0; i < 6; i++) {
            octaveLabels[i].doLayout(
                    keysPane.getRelativePosition().getLeft()
                            + keys.quantizeToNoteBorder(i * 12.0 * (1.0 / 61.0), VoiceGroup.Global) - 30,
                    keys.getPictureHeight() * 1.6, 50, 40);
        }

    }
}
