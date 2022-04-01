package com.nonlinearlabs.client.world.maps.settings;

import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.maps.ContextMenu;
import com.nonlinearlabs.client.world.maps.ContextMenuItem;
import com.nonlinearlabs.client.world.maps.MapsLayout;

public class OnOffContextMenu extends ContextMenu {

	public enum Items {
		ON, OFF;

		static public String toString(Items i) {
			switch (i) {
			case ON:
				return "On";

			case OFF:
				return "Off";

			default:
				return "";
			}
		}

		static public Items fromString(String str) {
			if (str != null) {
				if (str.toLowerCase().equals("on"))
					return ON;
				else if (str.toLowerCase().equals("off"))
					return OFF;
			}

			return ON;
		}
	}

	public interface Listener {
		public void setValue(Items item);
	}

	OnOffContextMenu(final Listener listener, MapsLayout parent) {
		super(parent);

		addChild(new ContextMenuItem(this, "On") {
			@Override
			public Control click(Position eventPoint) {
				listener.setValue(Items.ON);
				return super.click(eventPoint);
			}
		});

		addChild(new ContextMenuItem(this, "Off") {
			@Override
			public Control click(Position eventPoint) {
				listener.setValue(Items.OFF);
				return super.click(eventPoint);
			}
		});
	}
}