package com.nonlinearlabs.client.world;

import com.google.gwt.canvas.client.Canvas;
import com.google.gwt.event.dom.client.ContextMenuEvent;
import com.google.gwt.event.dom.client.ContextMenuHandler;
import com.google.gwt.event.dom.client.HumanInputEvent;
import com.google.gwt.event.dom.client.KeyDownEvent;
import com.google.gwt.event.dom.client.KeyDownHandler;
import com.google.gwt.event.dom.client.KeyUpEvent;
import com.google.gwt.event.dom.client.KeyUpHandler;
import com.google.gwt.event.dom.client.DomEvent.Type;
import com.google.gwt.event.shared.EventHandler;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.Window;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.pointer.PointerState;

public abstract class Mouseing {

	public static abstract class PointerEvent<H> {

		public static Type<Handler> eventType = new Type<Handler>(getEventString(), new Event());

		public static class Event extends HumanInputEvent<Handler> {

			@Override
			public Type<Handler> getAssociatedType() {
				return eventType;
			}

			@Override
			protected void dispatch(Handler handler) {
				handler.onEvent(this);
			}
		}

		public interface Handler extends EventHandler {
			void onEvent(Event event);
		}

		private static String getEventString() {
			if (H instanceof PointerDownEvent)
				return "pointerdown";

			return "";
		}
	}

	private static class PointerDownEvent extends PointerEvent<PointerDownEvent> {

	}

	private static class PointerUpEvent extends PointerEvent<PointerDownEvent> {
	}

	public Mouseing() {
	}

	public abstract void invalidate(int flags);

	public abstract void captureFocus();

	// var el = document.getElementsByTagName("canvas")[0];
	// el.addEventListener("pointerdown", handleStart, false);
	// el.addEventListener("pointerup", handleEnd, false);
	// el.addEventListener("pointercancel", handleCancel, false);
	// el.addEventListener("pointermove", handleMove, false);
	// log("initialized.");

	public void initHandlers(Canvas canvas) {
		Window.addResizeHandler(new NonMapsResizeHandler(this));
		// canvas.addMouseDownHandler(new NonMapsMouseDownHandler());
		// canvas.addMouseMoveHandler(new NonMapsMouseMoveHandler());
		// canvas.addMouseUpHandler(new NonMapsMouseUpHandler());
		// canvas.addTouchStartHandler(new NonMapsTouchStartHandler());
		// canvas.addTouchMoveHandler(new NonMapsTouchMoveHandler());
		// canvas.addTouchEndHandler(new NonMapsTouchEndHandler());
		canvas.addMouseWheelHandler(new NonMapsWheelHandler());
		// canvas.addDoubleClickHandler(new NonMapsMousDoubleClickHandler());

		canvas.addDomHandler((event) -> {
			event.preventDefault();
			event.stopPropagation();

			Position p = new Position(event.getNativeEvent().getClientX() * NonMaps.devicePixelRatio,
					event.getNativeEvent().getClientY() * NonMaps.devicePixelRatio);

			if (event.getNativeEvent().getButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_LEFT)
				PointerState.get().onLeftDown(p, event.isControlKeyDown());
			else if (event.getNativeEvent().getButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_RIGHT)
				PointerState.get().onRightDown(p);

			NonMaps.theMaps.captureFocus();
		}, PointerDownEvent.eventType);

		canvas.addDomHandler((event) -> {
			event.preventDefault();

			Position p = new Position(event.getNativeEvent().getClientX() * NonMaps.devicePixelRatio,
					event.getNativeEvent().getClientY() * NonMaps.devicePixelRatio);

			if (event.getNativeEvent().getButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_LEFT)
				PointerState.get().onLeftUp(p);
			else if (event.getNativeEvent().getButton() == com.google.gwt.dom.client.NativeEvent.BUTTON_RIGHT)
				PointerState.get().onRightUp(p);
		}, PointerUpEvent.eventType);

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