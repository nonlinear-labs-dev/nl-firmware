package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.canvas.client.Canvas;
import com.google.gwt.event.dom.client.ContextMenuEvent;
import com.google.gwt.event.dom.client.ContextMenuHandler;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.google.gwt.event.dom.client.KeyDownHandler;
import com.google.gwt.event.dom.client.KeyUpEvent;
import com.google.gwt.event.dom.client.KeyUpHandler;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.Window;
import com.nonlinearlabs.NonMaps.client.NonMaps;

public abstract class Mouseing {

	public Mouseing() {
	}

	public abstract void invalidate(int flags);

	public abstract void captureFocus();

	public void initHandlers(Canvas canvas) {
		Window.addResizeHandler(new NonMapsResizeHandler(this));
		canvas.addMouseDownHandler(new NonMapsMouseDownHandler());
		canvas.addMouseMoveHandler(new NonMapsMouseMoveHandler());
		canvas.addMouseUpHandler(new NonMapsMouseUpHandler());
		canvas.addTouchStartHandler(new NonMapsTouchStartHandler());
		canvas.addTouchMoveHandler(new NonMapsTouchMoveHandler());
		canvas.addTouchEndHandler(new NonMapsTouchEndHandler());
		canvas.addMouseWheelHandler(new NonMapsWheelHandler());
		canvas.addDoubleClickHandler(new NonMapsMousDoubleClickHandler());

		canvas.addHandler(new ContextMenuHandler() {
			@Override
			public void onContextMenu(ContextMenuEvent event) {
				event.preventDefault();
				event.stopPropagation();
			}
		}, ContextMenuEvent.getType());

		KeyDownHandler keypress = new KeyDownHandler() {
			@Override
			public void onKeyDown(KeyDownEvent event) {
				if (handleKeyPress(event)) {
					event.preventDefault();
					event.stopPropagation();
				}
			}
		};

		KeyUpHandler keyUpHandler = new KeyUpHandler() {
			@Override
			public void onKeyUp(KeyUpEvent event) {
				NonMaps.get().getNonLinearWorld().handleKeyUp(event);

			}
		};

		canvas.sinkEvents(Event.ONCONTEXTMENU | Event.KEYEVENTS);
		canvas.addKeyDownHandler(keypress);
		canvas.addKeyUpHandler(keyUpHandler);
		canvas.setFocus(true);
	}

	protected abstract boolean handleKeyPress(KeyDownEvent event);

}