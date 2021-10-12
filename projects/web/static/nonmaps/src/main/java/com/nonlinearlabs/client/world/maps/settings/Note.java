package com.nonlinearlabs.client.world.maps.settings;

public enum Note {
	C, Cis, D, Dis, E, F, Fis, G, Gis, A, As, B;

	public String toDisplayString() {
		switch (ordinal()) {
		case 0:
			return "C";

		case 1:
			return "C#";

		case 2:
			return "D";

		case 3:
			return "D#";

		case 4:
			return "E";

		case 5:
			return "F";

		case 6:
			return "F#";

		case 7:
			return "G";

		case 8:
			return "G#";

		case 9:
			return "A";

		case 10:
			return "A#";

		case 11:
			return "B";

		default:
			return "";
		}
	}

	public String toString() {
		return toDisplayString();
	}

	static public Note decode(String str) {
		for (Note i : values())
			if (str.equals(i.toString()))
				return i;

		return C;
	}

	static public boolean isSharp(Note i) {
		return i.toDisplayString().length() > 1;
	}
}