package com.nonlinearlabs.NonMaps.client.world;

public class Gray extends RGB {

	public Gray(int c) {
		super(c, c, c);
	}

	public static RGB overlayBackground() {
		return new Gray(40);
	}

}
