package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.*;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.NonDimension;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.overlay.DragProxy;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;

public class InsertTape extends MapsControl {


    private boolean inserting;
    private Bank prospectBank;
    private Tape.Orientation orientation;

    public InsertTape(Bank parent, Tape.Orientation orientation) {
        super(parent);
        this.setOrientation(orientation);
    }

    @Override
    public void doFirstLayoutPass(double levelOfDetail) {

    }

    @Override
    public Bank getParent() {
        return (Bank) super.getParent();
    }

    @Override
    public boolean isVisible() {

        Overlay o = getNonMaps().getNonLinearWorld().getViewport().getOverlay();

        for (DragProxy d : o.getDragProxies()) {
            Control r = d.getCurrentReceiver();
            if (r != null) {
                if (r instanceof PresetManager || r instanceof InsertTape || r instanceof Tape) {
                    return getParent().hasSlaveInDirection(getOrientation());
                }
            }
        }

        return false;

    }

    public Tape.Orientation getOrientation() {
        return orientation;
    }

    public void setOrientation(Tape.Orientation orientation) {
        this.orientation = orientation;
    }

    @Override
    public void draw(Context2d ctx, int invalidationMask) {
        super.draw(ctx, invalidationMask);
        if(inserting)
            getPixRect().fill(ctx, new RGB(250, 250, 250));
        else
            getPixRect().fill(ctx, new RGB(130,130,130));

        setShouldInsert(null, false);
    }

    public boolean fitsTo(Bank other) {
        if (getParent().isClusteredWith(other))
            return false;

        if (!isVisible())
            return false;

        return other.isVisible();
    }

    private void setShouldInsert(Bank b, boolean insert) {
        prospectBank = b;
        inserting = insert;
    }

    private boolean shouldInsert(Bank b) {
        return b == prospectBank && inserting;
    }

    @Override
    public Control drag(Rect pos, DragProxy dragProxy) {
        if (isVisible() && !getParent().isDraggingControl()) {
            if (dragProxy.getOrigin() instanceof Bank) {
                Bank other = (Bank) dragProxy.getOrigin();
                Position mouse = dragProxy.getMousePosition();

                if(getPixRect().contains(mouse)) {
                    if (getParent() != other) {
                        if (fitsTo(other)) {
                            //Display attaching!
                            setShouldInsert(other, true);
                            return this;
                        }
                    }
                }
                setShouldInsert(other, false);
            }
        }
        return super.drag(pos, dragProxy);
    }

    @Override
    public Control drop(Position pos, DragProxy dragProxy) {
        if (dragProxy.getOrigin() instanceof Bank) {
            Bank other = (Bank) dragProxy.getOrigin();
            if(shouldInsert(other)) {
                NonMaps.get().getServerProxy().insertBankInCluster(other, orientation, this.getParent());
                requestLayout();
            }
            return this;
        }
        return super.drop(pos, dragProxy);
    }
}
