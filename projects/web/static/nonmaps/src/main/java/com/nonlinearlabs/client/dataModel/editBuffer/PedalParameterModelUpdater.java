package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;

public class PedalParameterModelUpdater extends BasicParameterModelUpdater {

    public PedalParameterModelUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        super.doUpdate();
        PedalParameterModel p = (PedalParameterModel) target;
        p.mode.fromInt(Integer.parseInt(getChildText(root, "pedal-mode")));
    }
}
