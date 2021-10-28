package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.client.world.overlay.DragProxy;

public class PhysicalControlSendParameter extends PhysicalControlParameter {

    public PhysicalControlSendParameter(MapsLayout parent, int parameterID) {
        super(parent, parameterID);
    }

    //This does not work as I intended, maybe put a event-handler div on top of this control if presenter.isLocalDisabled 
    //, that handles all events instead of passing them down into the children controls
    
    @Override
    public void dragLeave() {
        if(!presenter.isLocalDisabled)
            super.dragLeave();
        return;
    }


    @Override
    public Control drop(Position pos, DragProxy dragProxy) {
        if(!presenter.isLocalDisabled)
            return super.drop(pos, dragProxy);
        return this;
    }


    @Override
    public Control longLeftPress(Position eventPoint) {
        if(!presenter.isLocalDisabled)
            return super.longLeftPress(eventPoint);
        return this;
    }


    @Override
    public Control longPress(Position eventPoint) {
        if(!presenter.isLocalDisabled)
            return super.longPress(eventPoint);
        return this;
    }


    @Override
    public Control longRightPress(Position eventPoint) {
        if(!presenter.isLocalDisabled)
            return super.longRightPress(eventPoint);
        return this;
    }


    @Override
    public Control mouseUp(Position eventPoint) {
        if(!presenter.isLocalDisabled)
            return super.mouseUp(eventPoint);
        return this;
    }
    
    @Override
    public Control mouseDown(Position eventPoint) {
        if(!presenter.isLocalDisabled)
            return super.mouseDown(eventPoint);
        return this;
    }

    @Override
    public Control onKey(KeyDownEvent event) {
        if(!presenter.isLocalDisabled)
           return super.onKey(event);
        return this;
    }
  
    @Override
    public Control doubleClick(Position pos) {
        if(!presenter.isLocalDisabled)
            return super.doubleClick(pos);
        return this;
    }
  
    @Override
    public Control click(Position eventPoint) {
        if(!presenter.isLocalDisabled)
            return super.click(eventPoint);
        return this;
    }
    
    @Override
    public Control drag(Position pos, DragProxy dragProxy) {
        if(!presenter.isLocalDisabled)
            return super.drag(pos, dragProxy);
        return this;
    }

    @Override
    public Control drag(Rect rect, DragProxy dragProxy) {
        if(!presenter.isLocalDisabled)
            return super.drag(rect, dragProxy);
        return this;
    }

    @Override
    public void draw(Context2d ctx, int invalidationMask) {
        if(!presenter.isLocalDisabled)
            super.draw(ctx, invalidationMask);
    }
}
