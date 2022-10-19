package com.nonlinearlabs.client.dataModel.editBuffer;

import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.NoSuchElementException;

import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;

public class ParameterFactory {
	static private int[] parameters = { 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 21, 23, 25, 27, 29, 31, 33, 34,
			35, 36, 37, 38, 40, 42, 44, 46, 48, 49, 50, 51, 52, 53, 55, 56, 57, 59, 60, 62, 63, 64, 66, 67, 68, 70, 71,
			73, 74, 75, 76, 78, 80, 81, 83, 85, 86, 87, 89, 90, 92, 93, 94, 96, 97, 98, 100, 101, 103, 104, 105, 106,
			108, 110, 111, 113, 115, 117, 118, 119, 121, 123, 125, 126, 127, 129, 131, 132, 133, 135, 136, 138, 140,
			142, 143, 144, 146, 147, 148, 150, 152, 153, 155, 156, 158, 160, 162, 164, 166, 167, 168, 169, 171, 172,
			174, 175, 177, 178, 180, 181, 183, 184, 185, 187, 188, 190, 191, 192, 194, 195, 196, 197, 199, 201, 203,
			204, 206, 207, 209, 211, 213, 214, 216, 218, 219, 221, 222, 223, 225, 227, 229, 231, 232, 233, 235, 237,
			238, 240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261,
			262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282,
			283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 299, 301, 302, 303, 304, 305,
			307, 308, 310, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 330,
			332, 334, 336, 338, 340, 342, 344, 346, 348, 350, 352, 354, 356, 358, 360, 362, 364, 365, 366, 367, 369,
			370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 391,
			393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 408, 409, 410, 411, 412, 413, 414,
			415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427 };

	static private int[] globalParameters = { 243, 244, 245, 246, 247, 248, 254, 255, 256, 257, 258, 259, 260, 261, 262,
			263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283,
			284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322,
			323, 324, 325, 326, 327, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383,	384, 
			385, 386, 387, 388, 391, 398, 399, 400, 401, 402, 403, 404, 405, 406, 408, 409, 410, 411, 412, 413, 414,
			415, 416, 417, 418, 419, 420, 421, 422, 423, 424 };

	static private int[] modulateableParameters = { 0, 2, 4, 6, 8, 10, 12, 19, 21, 23, 25, 27, 29, 31, 38, 40, 42, 44,
			46, 53, 55, 56, 57, 59, 60, 62, 63, 64, 66, 67, 68, 70, 71, 73, 74, 75, 76, 78, 80, 81, 83, 85, 86, 87, 89, 90, 92,
			93, 94, 96, 97, 98, 100, 101, 103, 104, 105, 106, 108, 110, 111, 113, 115, 117, 118, 119, 121, 123, 125, 126, 127,	129, 131, 132, 
			133, 135, 136, 138, 140, 142, 143, 144, 146, 147, 148, 150, 152, 153, 155, 156, 158, 160, 162, 164, 166, 167, 169, 171,
			172, 174, 175, 177, 178, 180, 181, 183, 184, 185, 188, 190, 191, 192, 194, 196, 197, 199, 201, 203, 204, 206,
			207, 209, 211, 213, 214, 216, 218, 219, 221, 222, 223, 225, 227, 229, 231, 232, 233, 235, 237, 238, 240, 241,
			247, 248, 250, 252, 253, 294, 295, 296, 297, 299, 301, 302, 303, 304, 305, 307, 308, 310, 313, 314, 315, 316,
			317, 318, 319, 320, 321, 322, 323, 332, 338, 342, 344, 346, 348, 350, 352, 354, 356, 358, 360, 362, 367, 389,
			391, 394, 406, 408, 427 };

	static private int[] pedals = { 254, 259, 264, 269 };
	public static int[] ribbons = { 284, 289, 409, 411 };

	static public int[] physicalControls = { 254, 259, 264, 269, 274, 279, 284, 289, 409, 411 };

	static public int[] sendParams = { 398, 399, 400, 401, 402, 403, 404, 405, 410, 412 };

	static private int[] macroControls = { 243, 244, 245, 246, 369, 371 };
	static private int[] modulationRouters = { 255, 256, 257, 258, 260, 261, 262, 263, 265, 266, 267, 268, 270, 271,
			272, 273, 275, 276, 277, 278, 280, 281, 282, 283, 285, 286, 287, 288, 290, 291, 292, 293, 373, 374, 375,
			376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 413, 414, 415, 416, 417, 418, 419, 420,
			421, 422, 423, 424 };

	static private int[] scaleOffsetParameters = { 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 391 };

	static private int[][] modRouters = { { 255, 256, 257, 258, 373, 374 }, { 260, 261, 262, 263, 375, 376 },
			{ 265, 266, 267, 268, 377, 378 }, { 270, 271, 272, 273, 379, 380 }, { 275, 276, 277, 278, 381, 382 },
			{ 280, 281, 282, 283, 383, 384 }, { 285, 286, 287, 288, 385, 386 }, { 290, 291, 292, 293, 387, 388 }, 
			{ 413, 414, 415, 416, 417, 418 }, { 419, 420, 421, 422, 423, 424 } };

	static private String[] parameterGroups = { "CS", "Cab", "Comb", "Echo", "Env A", "Env B", "Env C", "FB", "Flang",
			"Gap Filt", "MCM", "MCs", "Master", "Mixer", "Mono", "Osc A", "Osc B", "Part", "Reverb", "SVF", "Scale",
			"Sh A", "Sh B", "Split", "Unison" };
	static private String[] globalParameterGroups = { "CS", "MCM", "MCs", "Master", "Scale" };

	static public int[] voicesParameters = { 249, 250, 252, 253, 364, 365, 366, 367 };

	static public HashMap<SoundType, int[]> hiddenParametersBySoundType = new HashMap<SoundType, int[]>();
	static {
		final int[] layerHidden = { 356 };
		hiddenParametersBySoundType.put(SoundType.Layer, layerHidden);
		final int[] singleHidden = { 362, 348, 350, 352, 354, 396, 397, 406, 408 };
		hiddenParametersBySoundType.put(SoundType.Single, singleHidden);
		final int[] splitHidden = { 348, 350, 352, 396, 397 };
		hiddenParametersBySoundType.put(SoundType.Split, splitHidden);
	}

	static public HashMap<SoundType, int[]> disabledParametersBySoundType = new HashMap<SoundType, int[]>();
	static {
		final int[] layerDisabled = {};
		disabledParametersBySoundType.put(SoundType.Layer, layerDisabled);
		final int[] splitDisabled = { 346 };
		disabledParametersBySoundType.put(SoundType.Split, splitDisabled);
		final int[] singleDisabled = { 346 };
		disabledParametersBySoundType.put(SoundType.Single, singleDisabled);
	}

	static public BasicParameterModel create(ParameterId id) {
		if (Arrays.binarySearch(parameters, id.getNumber()) < 0)
			throw new NoSuchElementException();

		if (Arrays.binarySearch(scaleOffsetParameters, id.getNumber()) >= 0)
			return new ScaleOffsetParameterModel(id);

		if (Arrays.binarySearch(modulateableParameters, id.getNumber()) >= 0)
			return new ModulateableParameterModel(id);

		if (Arrays.binarySearch(pedals, id.getNumber()) >= 0)
			return new PedalParameterModel(id);

		if (Arrays.binarySearch(ribbons, id.getNumber()) >= 0)
			return new RibbonParameterModel(id);

		if (Arrays.binarySearch(sendParams, id.getNumber()) >= 0)
			return new SendParameterModel(id);

		if (id.getNumber() == 274)
			return new BenderParameterModel(id);

		if (id.getNumber() == 279)
			return new AftertouchParameterModel(id);

		if (Arrays.binarySearch(macroControls, id.getNumber()) >= 0)
			return new MacroControlParameterModel(id);

		if (Arrays.binarySearch(modulationRouters, id.getNumber()) >= 0)
			return new ModulationRouterParameterModel(id);

		return new BasicParameterModel(id);
	}

	static public int getNumberOfHW() {
		return physicalControls.length;
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

	public static int[] getSends() {
		return sendParams;
	}

	public static boolean isGlobalParameterGroup(String id) {
		return Arrays.binarySearch(globalParameterGroups, id) >= 0;
	}

	public static ParameterId[] getModulationRouters(ParameterId hw) {
		int physCtrlNum = ParameterFactory.hwToIdx(hw.getNumber());

		ParameterId result[] = new ParameterId[6];
		for (int i = 0; i < 6; i++)
			result[i] = new ParameterId(modRouters[physCtrlNum][i], VoiceGroup.Global);

		return result;
	}

	private static int hwToIdx(int number) {
		switch(number)
		{
			case 254:
				return 0;
			case 259:
				return 1;
			case 264:
				return 2;
			case 269:
				return 3;
			case 274:
				return 4;
			case 279:
				return 5; 
			case 284:
				return 6;
			case 289:
				return 7;
			case 409:
				return 8;
			case 411:
				return 9;
		}
		return 0;
	}

	public static ParameterId[] getModulationRoutersForMC(ParameterId id) {
		ParameterId result[] = new ParameterId[getNumberOfHW()];

		int mcPos = 0;

		for (int m : macroControls) {
			if (m == id.getNumber())
				break;

			mcPos++;
		}

		for (int i = 0; i < getNumberOfHW(); i++)
			result[i] = new ParameterId(modRouters[i][mcPos], VoiceGroup.Global);

		return result;
	}

	public static ParameterId getMacroControlForRouter(ParameterId id) {
		for (int[] row : modRouters) {
			int col = 0;
			for (int router : row) {
				if (router == id.getNumber())
					return new ParameterId(macroControls[col], VoiceGroup.Global);
				else
					col++;
			}
		}
		return null;
	}

	public static ParameterId getPhysicalControlForRouter(ParameterId id) {
		int p = 0;
		for (int[] row : modRouters) {
			for (int router : row) {
				if (router == id.getNumber())
					return new ParameterId(physicalControls[p], VoiceGroup.Global);
			}
			p++;
		}
		return null;
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


	private static boolean noDouble(int[] a) {
		HashSet<Integer> h = new HashSet<Integer>();
		for (int i = 0; i < a.length; i++)
		{
			if (h.contains(a[i]))
				return false;
			h.add(a[i]);
		}
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
		assert (isSorted(voicesParameters));

		assert (isSorted(parameterGroups));
		assert (isSorted(globalParameterGroups));
	}

	public static void assertNoDouble() {
		assert (noDouble(parameters));
		assert (noDouble(globalParameters));
		assert (noDouble(modulateableParameters));
		assert (noDouble(pedals));
		assert (noDouble(ribbons));
		assert (noDouble(macroControls));
		assert (noDouble(modulationRouters));
		assert (noDouble(scaleOffsetParameters));
		assert (noDouble(voicesParameters));
	}
}
