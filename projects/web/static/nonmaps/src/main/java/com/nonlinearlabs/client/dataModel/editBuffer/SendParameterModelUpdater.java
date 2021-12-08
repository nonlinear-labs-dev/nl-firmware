package com.nonlinearlabs.client.dataModel.editBuffer;

import com.google.gwt.core.client.GWT;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Checksum;

public class SendParameterModelUpdater extends BasicParameterModelUpdater  {

    public SendParameterModelUpdater(Node c, BasicParameterModel p) {
        super(c, p);
    }

    @Override
    public void doUpdate() {
        super.doUpdate();
        SendParameterModel p = (SendParameterModel) target;
        Checksum crc = new Checksum();
        p.getHash(crc);
        GWT.log("oldhash:" + crc.getHash());
        boolean isTrue = getChildText(root, "local-enabled").equals("1");
        p.enabled.setValue(isTrue);
        p.mode.fromInt(Integer.parseInt(getChildText(root, "return-mode")));
        Checksum ncrc = new Checksum();
        p.getHash(ncrc);
        GWT.log("newhash:" + ncrc.getHash());
    }
}
