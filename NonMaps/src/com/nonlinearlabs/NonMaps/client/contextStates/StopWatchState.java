package com.nonlinearlabs.NonMaps.client.contextStates;

public class StopWatchState implements AutoCloseable {

	// private Date start;
	// private String name;

	public StopWatchState(String name) {
		// start = new Date();
		// this.name = name;
	}

	@Override
	public void close() {
		// Date now = new Date();
		// long diff = now.getTime() - start.getTime();
		// Tracer.log("StopWatch " + name + " took " + diff + "ms");
	}
}
