package com.nonlinearlabs.client;

import java.util.LinkedList;

import com.google.gwt.animation.client.Animation;

public class Animator extends Animation {

	abstract public static class ClientData {
		public abstract void animate(double bent);
	}

	static public class DoubleClientData extends ClientData {
		public static interface Client {
			void animate(double v);
		}

		private Client c;
		private double from;
		private double to;

		@Override
		public void animate(double bent) {
			c.animate(from + (to - from) * bent);
		}
	}

	private LinkedList<ClientData> clients = new LinkedList<ClientData>();

	public Animator() {
		run(1500);
	}

	public Animator(int time) {
		run(time);
	}

	public void addSubAnimation(double from, double to, DoubleClientData.Client client) {
		DoubleClientData c = new DoubleClientData();
		c.c = client;
		c.from = from;
		c.to = to;
		clients.add(c);
	}

	@Override
	protected void onUpdate(double progress) {
		final double curve = 2;
		final double bent = 0.5 + Math.atan(2 * curve * progress - curve) / (2 * Math.atan(curve));

		for (ClientData c : clients)
			c.animate(bent);
	}
}
