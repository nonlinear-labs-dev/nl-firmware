package com.nonlinearlabs.client.world.overlay.html.overlayControlDialog;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.dom.client.Element;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.overlay.GWTDialog;
import com.nonlinearlabs.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.client.world.pointer.PointerState;

public class OverlayControlDialog extends GWTDialog {

    OverlayControlWrapper wrapper = null;

    public OverlayControlDialog(OverlayLayout origin, String className) {
        super();
        PointerState.get().resetGesture();
        getElement().addClassName(className);
        getElement().addClassName("overlay-dialog");
        add(wrapper = new OverlayControlWrapper(className));
        setGlassEnabled(false);
        setModal(false);
    }

    protected void commit() {
        hide();
        NonMaps.theMaps.captureFocus();
        NonMaps.theMaps.getNonLinearWorld().requestLayout();
    }

    @Override
    protected void setLastPopupPos(int popupLeft, int popupTop) {

    }

    @Override
    protected int getLastPopupPosTop() {
        return 0;
    }

    @Override
    protected int getLastPopupPosLeft() {
        return 0;
    }

    public Context2d getContext() {
        return wrapper.getContext();
    }

    public void sync(Control origin) {
        Rect rect = origin.getPixRect();
        Element e = getElement();
        double r = NonMaps.getDevicePixelRatio();

        e.getStyle().setPosition(com.google.gwt.dom.client.Style.Position.ABSOLUTE);
        e.getStyle().setPropertyPx("width", (int) (rect.getWidth() / r));
        e.getStyle().setPropertyPx("height", (int) (rect.getHeight() / r));
        e.getStyle().setPropertyPx("left", (int) (rect.getLeft() / r));
        e.getStyle().setPropertyPx("top", (int) (rect.getTop() / r));
        e.getStyle().setProperty("zIndex", "" + NonMaps.nextZIndex());

        wrapper.sync(origin);
    }
}
