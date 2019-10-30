package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;

public class PedalParameterUpdater extends ParameterUpdater {

    public PedalParameterUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        super.doUpdate();
        PedalParameterModel p = (PedalParameterModel) target;
        p.mode.fromString(getChildText(root, "pedal-mode"));
    }
}
