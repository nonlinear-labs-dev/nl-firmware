package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.nonlinearlabs.NonMaps.client.world.RGB;

public class DialogColorFactory {
	
	public static RGB getBackground() {
		return new RGB("#000000");
	}
	
	public static RGB getTextInputBackground() {
		return new RGB("#141414");
	}
	
	public static RGB getInactiveTab() {
		return new RGB("#1d1d1d");
	}
	
	public static RGB getActiveTab() {
		return new RGB("#202020");
	}
	
	public static RGB getTextField() {
		return new RGB("#202020");
	}
	
	/*
	Hintergrund: #000000 - ok
	Editfelder: #141414 - ok
	Inaktive Tabs: Setup: #1d1d1d - ok
	Inaktiver Tab: Preset Info: #202020 -->> #1d1d1d
	Aktiver Tab / Textfläche: Setup / Preset Info: #282828 -->> #202020
	Textfläche: Parameter Info / Search / Bank Info: #202020 - ok
	Textfläche: Undo History: #1a1a1a -->> #202020
	*/
}
