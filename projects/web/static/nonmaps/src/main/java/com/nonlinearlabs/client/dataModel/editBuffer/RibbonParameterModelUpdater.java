package com.nonlinearlabs.client.dataModel.editBuffer;
import com.google.gwt.xml.client.Node;

public class RibbonParameterModelUpdater extends BasicParameterModelUpdater {

    public RibbonParameterModelUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        super.doUpdate();
        RibbonParameterModel p = (RibbonParameterModel) target;
        p.mode.fromInt(Integer.parseInt(getChildText(root, "ribbon-return-mode")));
        p.behaviour.fromInt(Integer.parseInt(getChildText(root, "ribbon-touch-behaviour")));
    }
}
