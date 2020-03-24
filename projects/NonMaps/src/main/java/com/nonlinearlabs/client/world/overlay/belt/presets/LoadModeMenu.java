package com.nonlinearlabs.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.presenters.LocalSettingsProvider;
import com.nonlinearlabs.client.presenters.PresetManagerPresenterProvider;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.overlay.SVGImage;

public class LoadModeMenu extends OverlayLayout {

    private class DirectLoadButton extends SVGImage {
        DirectLoadButton(OverlayLayout parent) {
            super(parent, "Link_Enabled.svg", "Link_Active.svg");

            SetupModel.get().systemSettings.directLoad.onChange((v) -> {
                requestLayout();
                return true;
            });
        }

        private boolean isDirectLoadActive() {
            return SetupModel.get().systemSettings.directLoad.getBool();
        }

        @Override
        public int getSelectedPhase() {
            return isDirectLoadActive() ? 1 : 0;
        }
        
        @Override
        public Control click(Position eventPoint) {
            if(isDirectLoadActive()) {
                SetupModel.get().systemSettings.directLoad.setValue(false);
            } else {
                SetupModel.get().systemSettings.directLoad.setValue(true);
            }
            return this;
        }
    }

    private class LoadToPartButton extends SVGImage {
        LoadToPartButton(OverlayLayout parent) {
            super(parent, "Load_Disabled.svg", "Load_Enabled.svg", "Load_Active.svg");

            LocalSettingsProvider.get().getSettings().loadToPart.onChange((v) -> {
                requestLayout();
                return true;
            });
        }

        @Override
        public int getSelectedPhase() {
            if(!isDualEditBuffer()) {
                return 0;
            } else {
                if(isLoadToPartActive()) {
                    return 2;
                } else {
                    return 1;
                }
            }
        }

        private boolean isDualEditBuffer() {
            return EditBufferPresenterProvider.getPresenter().soundType != SoundType.Single;
        }

        private boolean isLoadToPartActive() {
            return LocalSettingsProvider.get().getSettings().loadToPart.getBool();
        }

        @Override
        public Control click(Position eventPosition) {
            if(isDualEditBuffer()) {
                if(isLoadToPartActive()) {
                    LocalSettingsProvider.get().getSettings().loadToPart.setValue(false);
                } else {
                    LocalSettingsProvider.get().getSettings().loadToPart.setValue(true);
                }
                return this;
            } else {
                return null;
            }
        }
    }

    private DirectLoadButton loadButton;
    private LoadToPartButton partButton;

    LoadModeMenu(OverlayLayout parent) {
        super(parent);

        SetupModel.get().systemSettings.directLoad.onChange((d) -> {
            requestLayout();
            return true;
        });

        LocalSettingsProvider.get().getSettings().loadToPart.onChange((v) -> {
            requestLayout();
            return true;
        });

        addChild(loadButton = new DirectLoadButton(this));
        addChild(partButton = new LoadToPartButton(this));
    }

    @Override
    public void draw(Context2d ctx, int invalidationMask) {
        super.draw(ctx, invalidationMask);
        getPixRect().drawDebug(ctx);
    }

    @Override
    public void doLayout(double x, double y, double w, double h) {
        double width = getSmallButtonWidth();
        double margin = Millimeter.toPixels(1);

        loadButton.doLayout(0, 0, width, width);
        partButton.doLayout(width + margin, 0, width, width);

        super.doLayout(x, y, w, h);
    }

    public double getDesiredWidth() {
        return getSmallButtonWidth() + Millimeter.toPixels(1) + getSmallButtonWidth();
    }

    public double getSmallButtonWidth() {
        return Millimeter.toPixels(11);
    }
}
