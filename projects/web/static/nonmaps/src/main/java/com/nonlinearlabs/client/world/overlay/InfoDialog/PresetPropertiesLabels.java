package com.nonlinearlabs.client.world.overlay.InfoDialog;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;

public class PresetPropertiesLabels {

	HTMLPanel panel;
    Label type;
    Label unison;
    Label mono;
    Label scale;

	public PresetPropertiesLabels() {
		panel = new HTMLPanel("div", "");
        panel.add(type = new Label());
        panel.add(unison = new Label());
        panel.add(mono = new Label());
        panel.add(scale = new Label());
		panel.getElement().addClassName("hashtag-div");

        for (Label label: new Label[]{type, unison, mono, scale}) {
            label.getElement().addClassName("preset-property-label");
        }
	}

	public HTMLPanel getHTML() {
		return panel;
	}

    public void updateFromPreset(String hashtagString)
    {
        GWT.log("updateFromPreset: <begin>" + hashtagString + "<end>");
        if(hashtagString.length() > 0)
        {
            var splits = hashtagString.split(",", 0);
            assert(splits.length == 4);
            var defaultString = "#Single, #Poly, #1-voice, #12-TET";
            var defaults = defaultString.split(",", 0);

            type.setText(splits[0]);
            unison.setText(splits[1]);
            mono.setText(splits[2]);
            scale.setText(splits[3]);

            type.getElement().removeClassName("preset-property-label-lowlight");
            if(defaults[0] == splits[0])
                type.getElement().addClassName("preset-property-label-lowlight");

            unison.getElement().removeClassName("preset-property-label-lowlight");
            if(defaults[1] == splits[1])
                unison.getElement().addClassName("preset-property-label-lowlight");

            mono.getElement().removeClassName("preset-property-label-lowlight");
            if(defaults[2] == splits[2])
                mono.getElement().addClassName("preset-property-label-lowlight");

            scale.getElement().removeClassName("preset-property-label-lowlight");
            if(defaults[3] == splits[3])
                scale.getElement().addClassName("preset-property-label-lowlight");
        }
        else
        {
            type.setText("");
            unison.setText("");
            mono.setText("");
            scale.setText("");
        }
    }
}
