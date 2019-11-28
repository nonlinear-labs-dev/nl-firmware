package com.nonlinearlabs.client.dataModel.editBuffer;

import java.util.Arrays;
import java.util.NoSuchElementException;

public class ParameterFactory {
	static private int[] parameters = { 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 21, 23, 25, 27, 29, 31, 33, 34,
			35, 36, 37, 38, 40, 42, 44, 46, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 60, 62, 63, 64, 66, 67, 68, 70, 71,
			73, 74, 75, 76, 78, 80, 81, 83, 83, 85, 86, 87, 89, 90, 92, 93, 94, 96, 97, 98, 100, 101, 103, 104, 105,
			106, 108, 110, 111, 113, 115, 117, 118, 119, 121, 122, 123, 125, 126, 127, 129, 131, 132, 133, 135, 136,
			138, 140, 142, 143, 144, 146, 147, 148, 150, 152, 153, 155, 156, 158, 160, 162, 164, 166, 167, 168, 169,
			171, 172, 174, 175, 177, 178, 180, 181, 183, 184, 185, 187, 188, 190, 191, 192, 194, 196, 197, 199, 201,
			203, 204, 206, 207, 209, 211, 213, 214, 216, 218, 219, 221, 222, 223, 225, 227, 229, 231, 232, 233, 235,
			237, 238, 240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 252, 253, 254, 255, 256, 257, 258, 259, 260,
			261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281,
			282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 299, 301, 302, 303, 304,
			305, 307, 308, 310, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 10001,
			10002, 10003 };

	static private int[] globalParameters = { 247, 248, 254, 259, 264, 269, 274, 279, 284, 289, 312, 313, 314, 315, 316,
			317, 318, 319, 320, 321, 322, 323, 10001 };

	static private int[] modulateableParameters = { 0, 2, 4, 6, 8, 10, 12, 19, 21, 23, 25, 27, 29, 31, 38, 40, 42, 44,
			46, 53, 57, 60, 64, 68, 71, 76, 78, 81, 83, 87, 90, 94, 98, 101, 106, 108, 111, 113, 115, 119, 123, 127,
			129, 133, 136, 138, 140, 144, 148, 150, 153, 156, 158, 160, 162, 164, 169, 172, 175, 178, 181, 185, 188,
			192, 194, 197, 199, 201, 204, 207, 209, 211, 214, 216, 219, 223, 225, 227, 229, 233, 235, 238, 241, 250,
			297, 299, 305, 308, 310 };

	static private int[] pedals = { 254, 259, 264, 269 };
	static private int[] ribbons = { 284, 289 };

	static private int[] macroControls = { 243, 244, 245, 246 };
	static private int[] modulationRouters = { 255, 256, 257, 258, 260, 261, 262, 263, 265, 266, 267, 268, 270, 271,
			272, 273, 275, 276, 277, 278, 280, 281, 282, 283, 285, 286, 287, 288, 290, 291, 292, 293 };
	static private int[] scaleOffsetParameters = { 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323 };

	static private String[] parameterGroups = { "CS", "Cab", "Comb", "Echo", "Env A", "Env B", "Env C", "FB", "Flang",
			"Gap Filt", "MCM", "MCs", "Master", "Mixer", "Mono", "Osc A", "Osc B", "PART", "Reverb", "SVF", "Scale", "Sh A",
			"Sh B", "Split", "Unison", "VGM" };
	static private String[] globalParameterGroups = { "CS", "Master", "Scale", "Split" };

	static public BasicParameterModel create(ParameterId id) {
		if (Arrays.binarySearch(parameters, id.getNumber()) < 0)
			throw new NoSuchElementException();

		if (Arrays.binarySearch(modulateableParameters, id.getNumber()) >= 0)
			return new ModulateableParameterModel(id);

		if (Arrays.binarySearch(pedals, id.getNumber()) >= 0)
			return new PedalParameterModel(id);

		if (Arrays.binarySearch(ribbons, id.getNumber()) >= 0)
			return new RibbonParameterModel(id);

		if (id.getNumber() == 274)
			return new BenderParameterModel(id);

		if (id.getNumber() == 279)
			return new AftertouchParameterModel(id);

		if (Arrays.binarySearch(macroControls, id.getNumber()) >= 0)
			return new MacroControlParameterModel(id);

		if (Arrays.binarySearch(modulationRouters, id.getNumber()) >= 0)
			return new ModulationRouterParameterModel(id);

		if (Arrays.binarySearch(scaleOffsetParameters, id.getNumber()) >= 0)
			return new ScaleOffsetParameterModel(id);

		return new BasicParameterModel(id);
	}

	static public int[] getModulationRouters() {
		return modulationRouters;
	}

	static public int[] getScaleOffsetParameters() {
		return scaleOffsetParameters;
	}

	static public int[] getGlobalParameters() {
		return globalParameters;
	}

	static public int[] getAllParameters() {
		return parameters;
	}

	public static boolean isGlobalParameter(int id) {
		return Arrays.binarySearch(globalParameters, id) >= 0;
	}

	public static String[] getParameterGroups() {
		return parameterGroups;
	}

	public static boolean isGlobalParameterGroup(String id) {
		return Arrays.binarySearch(globalParameterGroups, id) >= 0;
	}

	private static boolean isSorted(String[] a) {
		for (int i = 1; i < a.length; i++)
			if (a[i - 1].compareTo(a[i]) > 0)
				return false;

		return true;
	}

	private static boolean isSorted(int[] a) {
		for (int i = 1; i < a.length; i++)
			if (a[i - 1] > a[i])
				return false;

		return true;
	}

	public static void assertSorted() {
		assert (isSorted(parameters));
		assert (isSorted(globalParameters));
		assert (isSorted(modulateableParameters));
		assert (isSorted(pedals));
		assert (isSorted(ribbons));
		assert (isSorted(macroControls));
		assert (isSorted(modulationRouters));
		assert (isSorted(scaleOffsetParameters));

		assert (isSorted(parameterGroups));
		assert (isSorted(globalParameterGroups));
	}

}