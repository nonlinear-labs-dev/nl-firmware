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
}
