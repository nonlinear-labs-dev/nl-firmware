package com.nonlinearlabs.client.contextStates;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Rect;

public class ClipContext extends ContextState {

	public ClipContext(Context2d ctx, Rect r) {
		super(ctx);
		ctx.beginPath();
		ctx.rect(r.getLeft(), r.getTop(), r.getWidth(), r.getHeight());
		ctx.clip();
	}

	public ClipContext(Context2d ctx, Control c) {
		this(ctx, c.getPixRect());
	}

}
