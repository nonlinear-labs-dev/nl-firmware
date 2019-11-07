package com.nonlinearlabs.client;

public class Checksum {

	private long checksum = 0xFFFFFFFF;

	public Checksum() {

	}

	public void eat(double d) {
		eat(Math.round(d * 1000000));
	}

	public void eat(long v) {
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

	public void eat(String[] v) {
		for (String a : v)
			eat(a);
	}

	public long getHash() {
		return checksum;
	}

	public void eat(boolean v) {
		eat(v ? 13 : 19);
	}
}
