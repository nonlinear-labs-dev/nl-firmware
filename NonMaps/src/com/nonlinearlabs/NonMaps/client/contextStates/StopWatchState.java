package com.nonlinearlabs.NonMaps.client.contextStates;

import java.util.Date;

import com.nonlinearlabs.NonMaps.client.Tracer;

public class StopWatchState implements AutoCloseable {

	private Date start;
	private String name;

	public StopWatchState(String name) {
		start = new Date();
		this.name = name;
	}

	@Override
	public void close() {
		Date now = new Date();
		long diff = now.getTime() - start.getTime();
	}

}
