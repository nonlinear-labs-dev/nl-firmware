package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyUpEvent;
import com.google.gwt.event.dom.client.KeyUpHandler;
import com.google.gwt.event.dom.client.MouseDownEvent;
import com.google.gwt.event.dom.client.MouseDownHandler;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.user.client.Command;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.Event.NativePreviewEvent;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.CheckBox;
import com.google.gwt.user.client.ui.FlowPanel;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.Panel;
import com.google.gwt.user.client.ui.RadioButton;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.maps.NonRect;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;

public class SearchQueryDialog extends GWTDialog {

	private class NameFieldMouseUpHandler implements MouseDownHandler {
		TextBox nameField;

		public NameFieldMouseUpHandler(TextBox nameField) {
			this.nameField = nameField;
		}

		@Override
		public void onMouseDown(MouseDownEvent event) {
			nameField.setFocus(true);
		}
	}

	private class NameFieldKeyUpHandler implements KeyUpHandler {
		TextBox nameField;

		public NameFieldKeyUpHandler(TextBox nameField) {
			this.nameField = nameField;
		}

		@Override
		public void onKeyUp(KeyUpEvent event) {
			timer = new RepeatingCommand() {

				@Override
				public boolean execute() {
					if (timer == this) {
						String query = nameField.getText();
						NonMaps.theMaps.getNonLinearWorld().getPresetManager().setFilter(query);

						if (query.isEmpty())
							restoreZoom();
						else
							storeZoom();
					}
					return false;
				}
			};

			Scheduler.get().scheduleFixedDelay(timer, 250);
		}
	}

	private static SearchQueryDialog theDialog;
	private RepeatingCommand timer;
	private Label matches;
	private NonRect storedZoom;

	private SearchQueryDialog() {
		RootPanel.get().add(this);

		initalShow();

		addHeader("Search presets for ...");
		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);

		VerticalPanel rootPanel = new VerticalPanel();
		rootPanel.add(createTextInputPanel());
		rootPanel.add(createButtonPanel());
		setWidget(rootPanel);
	}

	private FlowPanel createButtonPanel() {
		FlowPanel buttonPanel = new FlowPanel();
		CheckBox zoomToAllMatches = new CheckBox("Zoom to all matches");
		RadioButton and = new RadioButton("AndOr", "AND");
		RadioButton or = new RadioButton("AndOr", "OR");
		zoomToAllMatches.getElement().addClassName("auto-zoom");
		Boolean b = Boolean.valueOf(NonMaps.theMaps.getNonLinearWorld().getSettings().get("search-auto-zoom", "false"));
		zoomToAllMatches.setValue(b);

		Button nextMatch = new Button("Next");
		Button previousMatch = new Button("Previous");

		previousMatch.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				highlightPreviousMatch();
			}
		});

		nextMatch.addClickHandler(new ClickHandler() {

			@Override
			public void onClick(ClickEvent event) {
				highlightNextMatch();
			}
		});

		zoomToAllMatches.addValueChangeHandler(new ValueChangeHandler<Boolean>() {

			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				NonMaps.theMaps.getNonLinearWorld().getSettings().set("search-auto-zoom", event.getValue().toString());

				if (event.getValue())
					zoomToAll();
			}
		});

		and.addValueChangeHandler(new ValueChangeHandler<Boolean>() {

			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				NonMaps.theMaps.getNonLinearWorld().getPresetManager().setSearchQueryCombination(PresetManager.SearchQueryCombination.AND);
			}
		});

		or.addValueChangeHandler(new ValueChangeHandler<Boolean>() {

			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				NonMaps.theMaps.getNonLinearWorld().getPresetManager().setSearchQueryCombination(PresetManager.SearchQueryCombination.OR);
			}
		});

		and.setValue(
				NonMaps.theMaps.getNonLinearWorld().getPresetManager().getSearchQueryCombination() == PresetManager.SearchQueryCombination.AND,
				false);
		or.setValue(
				NonMaps.theMaps.getNonLinearWorld().getPresetManager().getSearchQueryCombination() == PresetManager.SearchQueryCombination.OR,
				false);

		buttonPanel.add(previousMatch);
		buttonPanel.add(nextMatch);
		buttonPanel.add(zoomToAllMatches);
		buttonPanel.add(and);
		buttonPanel.add(or);
		return buttonPanel;
	}

	private Panel createTextInputPanel() {
		HorizontalPanel panel = new HorizontalPanel();

		final TextBox nameField = new TextBox();
		nameField.getElement().addClassName("search-query");
		nameField.addKeyUpHandler(new NameFieldKeyUpHandler(nameField));
		nameField.addMouseDownHandler(new NameFieldMouseUpHandler(nameField));

		nameField.addMouseDownHandler(new MouseDownHandler() {

			@Override
			public void onMouseDown(MouseDownEvent event) {
				double x = event.getRelativeX(nameField.getElement());
				double w = nameField.getElement().getClientWidth();
				double activeAreaWidth = Millimeter.toPixels(10) / (NonMaps.getPixelFactor() * NonMaps.devicePixelRatio);

				if (x > (w - activeAreaWidth)) {
					nameField.setText("");
					NonMaps.theMaps.getNonLinearWorld().getPresetManager().setFilter("");
					restoreZoom();
				}
			}
		});

		Scheduler.get().scheduleFinally(new Command() {
			public void execute() {
				nameField.setFocus(true);
			}
		});

		panel.add(nameField);
		panel.add(matches = new Label(""));
		updateMatches(0);
		return panel;
	}

	protected void highlightNextMatch() {
		NonMaps.theMaps.getNonLinearWorld().getPresetManager().highlightNextFilterMatch();
	}

	protected void highlightPreviousMatch() {
		NonMaps.theMaps.getNonLinearWorld().getPresetManager().highlightPreviousFilterMatch();
	}

	protected void zoomToAll() {
		NonMaps.theMaps.getNonLinearWorld().getPresetManager().zoomToAllFilterMatches();
	}

	@Override
	protected void onPreviewNativeEvent(NativePreviewEvent event) {
		super.onPreviewNativeEvent(event);
		switch (event.getTypeInt()) {
		case Event.ONKEYDOWN:
			if (event.getNativeEvent().getKeyCode() == KeyCodes.KEY_ESCAPE) {
				commit();
			} else if (event.getNativeEvent().getKeyCode() == KeyCodes.KEY_ENTER) {
				commit();
			}
			break;
		}
	}

	@Override
	protected void commit() {
		theDialog.hide();
		NonMaps.theMaps.getNonLinearWorld().getPresetManager().clearFilter();
		NonMaps.theMaps.captureFocus();
		theDialog = null;
		restoreZoom();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

	public static void toggle() {
		if (theDialog != null) {
			theDialog.commit();
		} else {
			if (!NonMaps.theMaps.getNonLinearWorld().getPresetManager().isEmpty())
				theDialog = new SearchQueryDialog();
		}
	}

	public static boolean isShown() {
		return theDialog != null;
	}

	public void updateMatches(int i) {
		if (i == 0)
			matches.setText("No matches.");
		else if (i == 1)
			matches.setText("One match.");
		else
			matches.setText(i + " matches.");
	}

	public static void setMatches(int i) {
		if (theDialog != null)
			theDialog.updateMatches(i);
	}

	private void storeZoom() {
		if (storedZoom == null)
			storedZoom = NonMaps.theMaps.getNonLinearWorld().getViewport().getNonPosition().copy();
	}

	private void restoreZoom() {
		if (storedZoom != null) {
			NonMaps.theMaps.getNonLinearWorld().animateViewport(storedZoom, true);
			storedZoom = null;
		}
	}

	static int lastPopupLeft = -1;
	static int lastPopupTop = -1;

	@Override
	protected void setLastPopupPos(int popupLeft, int popupTop) {
		lastPopupLeft = popupLeft;
		lastPopupTop = popupTop;
	}

	@Override
	protected int getLastPopupPosTop() {
		return lastPopupTop;
	}

	@Override
	protected int getLastPopupPosLeft() {
		return lastPopupLeft;
	}
}