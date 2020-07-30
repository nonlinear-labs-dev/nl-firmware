package com.nonlinearlabs.client;

import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.RGBA;

public class ColorTable {

	// GLOBAL COLORS

	public static RGB getColorLabel() {
		return new RGB(0, 0, 230);
	}

	public static RGB getColorSliderBackground() {
		return new Gray(25);
	}

	public static RGB getColorContour() {
		return new RGB(10, 10, 10);
	}

	public static RGB getColorIndicator() {
		return new RGB(250, 250, 250);
	}

	// MODULE SPECIFIC COLORS

	// --- Orange : OSC A / ENV A --- //

	public static RGB getColorEnvOscABackground() {
		return new RGB(56, 38, 28);
	}

	public static RGB getColorEnvOscAHeaderFont() {
		return new RGB(56, 38, 28);
	}

	public static RGB getColorEnvOscAHeaderBackground() {
		// return new RGB(255, 115, 0);
		return new RGB(242, 184, 136);
	}

	public static RGB getColorEnvOscASelectedBackground() {
		return new RGB(102, 71, 46);
	}

	public static RGB getColorEnvOscAFont() {
		// return new RGB(204, 173, 153);
		return new RGB(245, 245, 245);
	}

	// --- Gelb : OSC B / ENV B --- //

	public static RGB getColorEnvOscBBackground() {
		return new RGB(51, 49, 36);
	}

	public static RGB getColorEnvOscBHeaderFont() {
		return new RGB(51, 49, 36);
	}

	public static RGB getColorEnvOscBHeaderBackground() {
		// return new RGB(217, 173, 0);
		return new RGB(227, 212, 154);
	}

	public static RGB getColorEnvOscBSelectedBackground() {
		return new RGB(99, 88, 55);
	}

	public static RGB getColorEnvOscBFont() {
		// return new RGB(204, 188, 122);
		return new RGB(245, 245, 245);
	}

	// --- Grï¿½n : Comb Filter --- //

	public static RGB getColorCombBackground() {
		return new RGB(33, 48, 36);
	}

	public static RGB getColorEnvCombHeaderFont() {
		return new RGB(33, 48, 36);
	}

	public static RGB getColorCombHeaderBackground() {
		// return new RGB(82, 182, 32);
		return new RGB(186, 230, 189);
	}

	public static RGB getColorCombSelectedBackground() {
		return new RGB(72, 99, 76);
	}

	public static RGB getColorCombFont() {
		// return new RGB(172, 204, 169);
		return new RGB(245, 245, 245);
	}

	// --- Türkis : SVF Filter --- //

	public static RGB getColorSVFBackground() {
		return new RGB(34, 46, 43);
	}

	public static RGB getColorEnvSVFHeaderFont() {
		return new RGB(34, 46, 43);
	}

	public static RGB getColorSVFHeaderBackground() {
		// return new RGB(0, 170, 158);
		return new RGB(171, 216, 222);
	}

	public static RGB getColorSVFSelectedBackground() {
		return new RGB(66, 89, 87);
	}

	public static RGB getColorSVFFont() {
		return new RGB(245, 245, 245);
	}

	// --- Blau : EFFECTS --- //

	public static RGB getColorFXBackground() {
		return new RGB(41, 44, 59);
	}

	public static RGB getColorEnvFXHeaderFont() {
		return new RGB(41, 44, 59);
	}

	public static RGB getColorFXHeaderBackground() {
		// return new RGB(128, 149, 255);
		return new RGB(191, 202, 255);
	}

	public static RGB getColorFXSelectedBackground() {
		return new RGB(76, 83, 117);
	}

	public static RGB getColorFXFont() {
		// return new RGB(173, 181, 217);
		return new RGB(245, 245, 245);
	}

	// --- Grau : OUTPUT MIXER / AUX --- //

	public static RGB getColorAUXBackground() {
		return new RGB(44, 45, 46);
	}

	public static RGB getColorEnvAUXHeaderFont() {
		return new RGB(44, 45, 46);
	}

	public static RGB getColorAUXSelectedBackground() {
		return new RGB(85, 87, 89);
	}

	public static RGB getColorAUXFont() {
		return new RGB(245, 245, 245);
	}

	// --- Rot : FEEDBACK --- //

	public static RGB getColorFeedBackBackground() {
		return new RGB(56, 52, 69);
	}

	public static RGB getColorEnvFeedBackHeaderFont() {
		return new RGB(51, 32, 31);
	}

	public static RGB getColorFeedBackHeaderBackground() {
		return new RGB(190, 186, 229);
	}

	public static RGB getColorFeedBackSelectedBackground() {
		return new RGB(92, 91, 107);
	}

	public static RGB getColorFeedBackFont() {
		return new RGB(245, 245, 245);
	}

	public static RGB getDisabledStateShadingColor() {
		return new RGBA(0, 0, 0, 0.75);
	}

	public static RGB getMacroControlTargetColor() {
		return new RGB(204, 14, 43);
	}
}