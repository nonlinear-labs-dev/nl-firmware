package com.nonlinearlabs.client.world.maps.parameters.PlayControls.SourcesAndAmounts.Sources;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.Tracer;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.presenters.ParameterPresenter;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.maps.MapsLayout;
import com.nonlinearlabs.client.world.maps.NonDimension;
import com.nonlinearlabs.client.world.maps.NonRect;
import com.nonlinearlabs.client.world.maps.parameters.PhysicalControlParameter;
import com.nonlinearlabs.client.world.overlay.DragProxy;

public class PhysicalControlSendParameter extends PhysicalControlParameter {

    public PhysicalControlSendParameter(MapsLayout parent, int parameterID) {
        super(parent, parameterID);
    }
    
    @Override
    public void dragLeave() {
        if(presenter.isLocalDisabled)
            super.dragLeave();
        return;
    }

    @Override
    protected boolean onPresenterUpdated(ParameterPresenter p)
    {
        GWT.log("Invalidating SendParameter " + p.id.toString());
        if(p != presenter)
        {
            presenter = p;
            getParameterGroup().requestLayout();
            invalidate(INVALIDATION_FLAG_ZOOMED);
        }
        return true;
    }

    @Override
    public double getXMargin() {
        if(presenter.isLocalDisabled)
            return super.getXMargin();
        return 0;
    }

    @Override
    public double getLeftMargin() {
        if(presenter.isLocalDisabled)
            return super.getLeftMargin();
        return 0;
    }

    @Override
    public double getRightMargin() {
        if(presenter.isLocalDisabled)
            return super.getRightMargin();
        return 0;
    }

    @Override
    public double getWidthMargin() {
        if(presenter.isLocalDisabled)
            return super.getWidthMargin();
        return 0;
    }

    private int getSibling()
    {
        switch(presenter.id.getNumber())
        {
            case 398:
                return 254;
            case 399:
                return 259;
            case 400: 
                return 264;
            case 401:
                return 269;
            case 402:
                return 274;
            case 403:
                return 279;
            case 404:
                return 284;
            case 405:
                return 289;
        }
        return -1;
    }

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		if (!presenter.isLocalDisabled) {
            NonRect dim = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().findParameter(getSibling()).getNonPosition();
            setNonSize(new NonDimension(0, dim.getHeight()));
		} else {
			super.doFirstLayoutPass(levelOfDetail);
		}
	}

    @Override
    public Control drop(Position pos, DragProxy dragProxy) {
        if(presenter.isLocalDisabled)
            return super.drop(pos, dragProxy);
        return this;
    }


    @Override
    public Control longLeftPress(Position eventPoint) {
        if(presenter.isLocalDisabled)
            return super.longLeftPress(eventPoint);
        return this;
    }


    @Override
    public Control longPress(Position eventPoint) {
        if(presenter.isLocalDisabled)
            return super.longPress(eventPoint);
        return this;
    }


    @Override
    public Control longRightPress(Position eventPoint) {
        if(presenter.isLocalDisabled)
            return super.longRightPress(eventPoint);
        return this;
    }


    @Override
    public Control mouseUp(Position eventPoint) {
        if(presenter.isLocalDisabled)
            return super.mouseUp(eventPoint);
        return this;
    }
    
    @Override
    public Control mouseDown(Position eventPoint) {
        if(presenter.isLocalDisabled)
            return super.mouseDown(eventPoint);
        return this;
    }

    @Override
    public Control onKey(KeyDownEvent event) {
        if(presenter.isLocalDisabled)
           return super.onKey(event);
        return this;
    }
  
    @Override
    public Control doubleClick(Position pos) {
        if(presenter.isLocalDisabled)
            return super.doubleClick(pos);
        return this;
    }
  
    @Override
    public Control click(Position eventPoint) {
        if(presenter.isLocalDisabled)
            return super.click(eventPoint);
        return this;
    }
    
    @Override
    public Control drag(Position pos, DragProxy dragProxy) {
        if(presenter.isLocalDisabled)
            return super.drag(pos, dragProxy);
        return this;
    }

    @Override
    public Control drag(Rect rect, DragProxy dragProxy) {
        if(presenter.isLocalDisabled)
            return super.drag(rect, dragProxy);
        return this;
    }

    @Override
    public void draw(Context2d ctx, int invalidationMask) {
        if(presenter.isLocalDisabled)
            super.draw(ctx, invalidationMask);
    }
}
