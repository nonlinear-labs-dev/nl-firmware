package com.nonlinearlabs.NonMaps.client;

public class Checksum {

	private long checksum = 0xFFFFFFFF;

	public Checksum() {

	}

	public void eat(double d) {
		eat(Math.round(d * 1000000));
	}

	private void eat(long v) {
		final int prime = 31;
		checksum = prime * checksum + v;
	}

	public void eat(int v) {
		final int prime = 43;
		checksum = prime * checksum + v;
	}

	public void eat(String v) {
		eat(v.hashCode());
	}

	public long getHash() {
		return checksum;
	}

	public void eat(boolean v) {
		eat(v ? 13 : 19);
	}
}
