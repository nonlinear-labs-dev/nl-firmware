package com.nonlinearlabs.client.world.maps.settings;

import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.NonPosition;

public class PopoutOleds extends Setting {

    protected PopoutOleds(DeveloperSettings parent) {
        super(parent, "Pop-out OLEDs", "Go!");
    }

    @Override
    public Control onValueButtonMouseDown(Position eventPoint) {
        Window.open("nonmaps/mirror-oleds.html", "_blank", "");;
        return this;
    }

    @Override
    public ContextMenu createMenu(NonPosition pos) {
        return null;
    }

    @Override
    public void setDefault() {
    }

}