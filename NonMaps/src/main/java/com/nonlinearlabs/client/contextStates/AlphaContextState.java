package com.nonlinearlabs.client.contextStates;

import com.google.gwt.canvas.dom.client.Context2d;

public class AlphaContextState extends ContextState {

	public AlphaContextState(Context2d ctx, double alpha) {
		super(ctx);
		ctx.setGlobalAlpha(alpha);
	}

}
