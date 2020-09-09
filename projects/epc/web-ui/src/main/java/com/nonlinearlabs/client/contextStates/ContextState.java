package com.nonlinearlabs.client.contextStates;

import com.google.gwt.canvas.dom.client.Context2d;

public class ContextState implements AutoCloseable {

	private Context2d ctx;

	public ContextState(Context2d ctx) {
		this.ctx = ctx;
		ctx.save();
	}

	@Override
	public void close() {
		ctx.restore();
	}

}
