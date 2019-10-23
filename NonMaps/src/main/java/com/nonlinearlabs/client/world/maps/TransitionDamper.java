package com.nonlinearlabs.client.world.maps;

import java.util.Date;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;

public class TransitionDamper {

	private int frequency = 50;
	private double x;
	private double y;
	private double filter;
	private boolean filterRunning = false;
	private Client client;
	long timeStamp;

	public interface Client {
		void moveBy(double x, double y);
	}

	public TransitionDamper(double filter, Client client) {
		activate(0, 0);
		this.filter = filter;
		this.client = client;
	}

	private void runFilter() {

		if (filterRunning)
			return;

		filterRunning = true;

		Scheduler.get().scheduleFixedPeriod(new RepeatingCommand() {

			@Override
			public boolean execute() {
				if (filterRunning) {
					filter();

					filterRunning = (Math.abs(x) >= 0.5) || (Math.abs(y) >= 0.5);

					if (filterRunning)
						client.moveBy(x, y);
				}

				return filterRunning;
			}
		}, 1000 / frequency);
	}

	public void stop() {
		x = y = 0;
	}

	public void activate(double x, double y) {
		this.x = x;
		this.y = y;
		filterRunning = false;
		this.timeStamp = new Date().getTime();
	}

	private void filter() {
		this.x *= this.filter;
		this.y *= this.filter;
	}

	public void run() {
		Date now = new Date();
		long diff = now.getTime() - timeStamp;

		if (diff < 250)
			runFilter();
	}
}
