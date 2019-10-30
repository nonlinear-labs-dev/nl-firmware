package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;

public class RibbonParameterUpdater extends ParameterUpdater {

    public RibbonParameterUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        super.doUpdate();
        RibbonParameterModel p = (RibbonParameterModel) target;
        p.mode.fromString(getChildText(root, "ribbon-return-mode"));
    }
}
