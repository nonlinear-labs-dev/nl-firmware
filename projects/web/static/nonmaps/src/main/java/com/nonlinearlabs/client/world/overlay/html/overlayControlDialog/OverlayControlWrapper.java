package com.nonlinearlabs.client.world.overlay.html.overlayControlDialog;

import com.google.gwt.canvas.client.Canvas;
import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.Element;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Rect;

public class OverlayControlWrapper extends Composite {

    Canvas canvasWidget = Canvas.createIfSupported();

    @UiField
    HTMLPanel panel;

    interface SetupUiBinder extends UiBinder<HTMLPanel, OverlayControlWrapper> {
    }

    private static SetupUiBinder ourUiBinder = GWT.create(SetupUiBinder.class);

    public OverlayControlWrapper(String wrappedClass) {
        initWidget(ourUiBinder.createAndBindUi(this));
        panel.getElement().addClassName(wrappedClass + "-wrapper");
        panel.getElement().addClassName("overlay-dialog-wrapper");
        panel.add(canvasWidget);
    }

    @Override
    protected void onAttach() {
        super.onAttach();
        NonMaps.get().initHandlers(canvasWidget);
    }

    public Context2d getContext() {
        return canvasWidget.getContext2d();
    }

    public void sync(Control origin) {
        canvasWidget.setCoordinateSpaceWidth((int) origin.getPixRect().getWidth() + 1);
        canvasWidget.setCoordinateSpaceHeight((int) origin.getPixRect().getHeight() + 1);

        double r = NonMaps.getDevicePixelRatio();
        Element e = canvasWidget.getElement();
        Rect rect = origin.getPixRect();
        e.getStyle().setPropertyPx("width", (int) (rect.getWidth() / r));
        e.getStyle().setPropertyPx("height", (int) (rect.getHeight() / r));
    }
}
