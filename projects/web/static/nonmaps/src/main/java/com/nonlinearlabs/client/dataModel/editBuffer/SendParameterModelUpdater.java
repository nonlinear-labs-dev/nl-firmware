package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.dataModel.editBuffer.BasicParameterModelUpdater;

public class SendParameterModelUpdater extends BasicParameterModelUpdater  {

    public SendParameterModelUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        super.doUpdate();
        SendParameterModel p = (SendParameterModel) target;
        boolean isTrue = getChildText(root, "local-enabled").equals("1");
        p.enabled.setValue(isTrue);
    }
}
