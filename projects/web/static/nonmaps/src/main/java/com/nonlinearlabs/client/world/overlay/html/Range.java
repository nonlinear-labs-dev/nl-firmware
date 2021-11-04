package com.nonlinearlabs.client.world.overlay.html;

import com.google.gwt.dom.client.BrowserEvents;
import com.google.gwt.dom.client.Element;
import com.google.gwt.dom.client.InputElement;
import com.google.gwt.event.dom.client.DomEvent;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.event.shared.EventHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.user.client.ui.HasValue;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.Widget;

public class Range extends Widget implements HasValue<Number> {
	public interface InputHandler extends EventHandler {
		void onInput(InputEvent event);
	}

	public static class InputEvent extends DomEvent<InputHandler> {
		private static final Type<InputHandler> TYPE = new Type<InputHandler>(BrowserEvents.INPUT, new InputEvent());

		public static Type<InputHandler> getType() {
			return TYPE;
		}

		protected InputEvent() {
		}

		@Override
		public final Type<InputHandler> getAssociatedType() {
			return TYPE;
		}

		@Override
		protected void dispatch(InputHandler handler) {
			handler.onInput(this);
		}
	}

	public static Range wrap(Element element) {
		Range range = new Range(element);
		range.onAttach();
		RootPanel.detachOnWindowClose(range);
		return range;
	}

	protected Range(Element element) {
		assert InputElement.as(element).getType().equalsIgnoreCase("range");
		setElement(element);

		addBitlessDomHandler(new InputHandler() {

			@Override
			public void onInput(InputEvent event) {
				ValueChangeEvent.fire(Range.this, getValue());

			}
		}, InputEvent.TYPE);
	}

	public Number getValue() {
		String value = getInputElement().getValue();
		return Double.valueOf(value);
	}

	public void setValue(Number value) {
		setValue(value, false);
	}

	public final native void setValue(Element element, Number value) /*-{
																		element.value = value;
																		}-*/;

	private InputElement getInputElement() {
		return getElement().cast();
	}

	@Override
	public void setValue(Number value, boolean fireEvents) {
		if (value == null) {
			value = 0;
		}

		Number oldValue = getValue();
		setValue(getInputElement(), value);

		if (value.equals(oldValue)) {
			return;
		}

		if (fireEvents) {
			ValueChangeEvent.fire(this, value);
		}
	}

	@Override
	public HandlerRegistration addValueChangeHandler(ValueChangeHandler<Number> handler) {
		return addHandler(handler, ValueChangeEvent.getType());
	}

}
