package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.core.client.GWT;
import com.google.gwt.xml.client.Node;

public class SendParameterModelUpdater extends BasicParameterModelUpdater  {

    public SendParameterModelUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        GWT.log("doUpdate for" + target.getValue().longName.getValue());
        super.doUpdate();
        SendParameterModel p = (SendParameterModel) target;
        boolean isTrue = getChildText(root, "local-enabled").equals("1");
        p.enabled.setValue(isTrue);
        p.mode.fromInt(Integer.parseInt(getChildText(root, "return-mode")));
    }
}
