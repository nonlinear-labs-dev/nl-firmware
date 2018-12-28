package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset;

import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;

public class PresetColorPack {
		public PresetColorPack(RGB c, RGB f, RGB h) {
				contour = c;
				fill = f;
				highlight = h;
				overlay = new RGBA(0, 0, 0, 0);
			}
		
		public PresetColorPack applyNotMatched(PresetColorPack current) {
			current.overlay =  new RGBA(0, 0, 0, 0.75);
			return current;
		}
		
		public RGB contour;
		public RGB fill;
		public RGB highlight;
		public RGBA overlay;
	}