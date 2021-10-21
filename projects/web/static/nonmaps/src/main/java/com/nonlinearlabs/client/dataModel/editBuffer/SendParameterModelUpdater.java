package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;

public class SendParameterModelUpdater extends BasicParameterModelUpdater  {

    public SendParameterModelUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        super.doUpdate();
        SendParameterModel p = (SendParameterModel) target;
        p.enabled.fromString(getChildText(root, "is-enabled"));
    }    
}
