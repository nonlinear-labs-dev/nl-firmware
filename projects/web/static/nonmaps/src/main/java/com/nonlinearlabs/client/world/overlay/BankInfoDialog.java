package com.nonlinearlabs.client.world.overlay;

import java.util.Date;

import com.google.gwt.event.dom.client.BlurEvent;
import com.google.gwt.event.dom.client.BlurHandler;
import com.google.gwt.event.dom.client.FocusEvent;
import com.google.gwt.event.dom.client.FocusHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyPressEvent;
import com.google.gwt.event.dom.client.KeyPressHandler;
import com.google.gwt.event.dom.client.MouseDownEvent;
import com.google.gwt.event.dom.client.MouseDownHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.i18n.client.DateTimeFormat.PredefinedFormat;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.Event.NativePreviewEvent;
import com.google.gwt.user.client.Event.NativePreviewHandler;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.IntegerBox;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextArea;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.Widget;
import com.nonlinearlabs.client.GMTTimeZone;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;

public class BankInfoDialog extends GWTDialog {

	private static BankInfoDialog theDialog;

	private static float commentBoxHeight = 0;

	private TextBox name;
	private IntegerBox position;
	private TextArea comment;
	private Label size;
	private Label lastChange;
	private Label importFileDate;
	private Label importFileName;
	private Label stateLabel, exportFileName, exportFileDate;

	Bank theBank;

	private Widget haveFocus = null;

	private BankInfoDialog() {
		RootPanel.get().add(this);

		getElement().addClassName("bank-info-dialog");

		initalShow();

		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);

		addHeader("Bank Info");
		addContent();

		initialSetup();

		super.pushDialogToFront();

		if (commentBoxHeight != 0) {
			comment.setHeight(commentBoxHeight + "px");
		}
	}

	private void initialSetup() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		updateInfo(pm.findBank(pm.getSelectedBank()));
	}

	private void addRow(FlexTable panel, String name, Widget content) {
		int c = panel.getRowCount();
		panel.setWidget(c, 0, new Label(name));
		panel.setWidget(c, 1, content);
	}

	private void addContent() {
		HTMLPanel bankNameAndPositionBox = new HTMLPanel("div", "");
		bankNameAndPositionBox.getElement().addClassName("preset-name-and-pos");
		bankNameAndPositionBox.add(position = new IntegerBox());
		bankNameAndPositionBox.add(name = new TextBox());
		position.getElement().addClassName("position-box editable");
		name.getElement().addClassName("preset-name-box editable");

		position.setText("1");

		FlexTable panel = new FlexTable();
		addRow(panel, "Position/Name", bankNameAndPositionBox);
		addRow(panel, "Comment", comment = new TextArea());
		addRow(panel, "Size", size = new Label(""));
		addRow(panel, "State", stateLabel = new Label(""));
		addRow(panel, "Last Change", lastChange = new Label(""));
		addRow(panel, "Import Date", importFileDate = new Label(""));
		addRow(panel, "Import File", importFileName = new Label(""));
		addRow(panel, "Export Date", exportFileDate = new Label(""));
		addRow(panel, "Export File", exportFileName = new Label(""));

		position.getElement().addClassName("gwt-TextBox");

		comment.getElement().addClassName("editable");
		comment.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				setFocus(comment);
			}
		});

		comment.addMouseDownHandler(new MouseDownHandler() {
			private HandlerRegistration mouseMoveUpRegistration;
			private int lastWidth;
			private int lastHeight;

			@Override
			public void onMouseDown(MouseDownEvent event) {
				lastWidth = getOffsetWidth();
				lastHeight = getOffsetHeight();

				if (mouseMoveUpRegistration == null) {
					mouseMoveUpRegistration = Event.addNativePreviewHandler(new NativePreviewHandler() {
						@Override
						public void onPreviewNativeEvent(NativePreviewEvent event) {
							if (event.getTypeInt() == Event.ONMOUSEMOVE || event.getTypeInt() == Event.ONMOUSEUP) {
								int width = getOffsetWidth();
								int height = getOffsetHeight();
								if (width != lastWidth || height != lastHeight) {
									commentBoxHeight = comment.getElement().getClientHeight();

									lastWidth = width;
									lastHeight = height;
								}

								if (event.getTypeInt() == Event.ONMOUSEUP) {
									commentBoxHeight = comment.getElement().getClientHeight();

									if (mouseMoveUpRegistration != null) {
										mouseMoveUpRegistration.removeHandler();
										mouseMoveUpRegistration = null;
									}
								}
							}
						}
					});
				}
			}
		});

		comment.addBlurHandler(new BlurHandler() {

			@Override
			public void onBlur(BlurEvent event) {
				haveFocus = null;

				if (theBank != null) {
					String oldInfo = theBank.getAttribute("Comment");

					if (!oldInfo.equals(comment.getText())) {
						NonMaps.theMaps.getServerProxy().setBankAttribute(theBank, "Comment", comment.getText());
					}
				}
			}
		});

		comment.addKeyUpHandler(event -> {
			updateCommentFieldHeight();
		});

		position.addKeyPressHandler(new KeyPressHandler() {

			@Override
			public void onKeyPress(KeyPressEvent event) {
				if (event.getCharCode() == KeyCodes.KEY_ENTER) {
					position.setFocus(false);
					name.setFocus(true);
				}
			}
		});

		position.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				setFocus(position);
			}
		});

		position.addBlurHandler(new BlurHandler() {

			@Override
			public void onBlur(BlurEvent event) {
				haveFocus = null;

				if (theBank != null) {
					int oldNumber = theBank.getOrderNumber();
					int currentValue = position.getValue();

					if (oldNumber != currentValue) {
						NonMaps.get().getServerProxy().setBankOrderNumber(theBank, currentValue);
					}
				}
			}
		});

		name.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				setFocus(name);
			}
		});

		name.addBlurHandler(new BlurHandler() {

			@Override
			public void onBlur(BlurEvent event) {
				haveFocus = null;

				if (theBank != null) {
					String oldName = theBank.getCurrentName();

					if (!oldName.equals(name.getText())) {
						NonMaps.theMaps.getServerProxy().renameBank(theBank.getUUID(), name.getText());
					}
				}
			}
		});

		name.addKeyPressHandler(new KeyPressHandler() {

			@Override
			public void onKeyPress(KeyPressEvent arg0) {
				if (arg0.getCharCode() == KeyCodes.KEY_ENTER) {
					name.setFocus(false);
					comment.setFocus(true);
				}
			}
		});

		setWidget(panel);
		setFocus(panel);

		//for some reason we can't properly initialize the height of the comment section on the first open so we have to recalculate it here after some time
		Timer t = new Timer() {
			@Override
			public void run() {
				updateCommentFieldHeight();
			}
		};
		t.schedule(15);
	}

	@Override
	protected void commit() {
		hide();
		theDialog = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

	public static void toggle() {
		if (theDialog != null) {
			theDialog.commit();
		} else {
			if (NonMaps.theMaps.getNonLinearWorld().getPresetManager().getSelectedBank() != null)
				theDialog = new BankInfoDialog();
		}
	}

	public static boolean isShown() {
		return theDialog != null;
	}

	public static void update(Bank bank) {
		if (theDialog != null)
			theDialog.updateInfo(bank);
	}

	private void updateCommentFieldHeight()	{
		if (comment.getElement().getScrollHeight() > 0) {
			comment.setHeight("1em");
			int height = comment.getElement().getScrollHeight() + 10;
			comment.setHeight(height + "px");
		}
	}

	private void updateInfo(Bank bank) {
		if (bank != null) {
			String bankName = bank.getCurrentName();
			String commentText = bank.getAttribute("Comment");
			int bankPos = bank.getOrderNumber();

			if (haveFocus != comment) {
				if (!commentText.equals(comment.getText())) {
					comment.setText(commentText);
					updateCommentFieldHeight();
				}
			}

			if (haveFocus != name) {
				if (!bankName.equals(name.getText())) {
					name.setText(bankName);
				}
			}

			Integer currentPositionValue = position.getValue();

			if (haveFocus != position) {
				if (bankPos != currentPositionValue) {
					position.setText(Integer.toString(bankPos));
				}
			}

			size.setText(Integer.toString(bank.getPresetList().getPresetCount()));

			try {
				lastChange.setText(localizeTime(bank.getDateOfLastChange()));
			} catch (Exception e) {
				lastChange.setText("---");
			}

			try {
				importFileDate.setText(localizeTime(bank.getAttribute("Date of Import File")));
			} catch (Exception e) {
				importFileDate.setText("---");
			}

			try {
				exportFileDate.setText(localizeTime(bank.getAttribute("Date of Export File")));
			} catch (Exception e) {
				exportFileDate.setText("---");
			}

			try {
				exportFileName.setText(localizeTime(bank.getAttribute("Name of Export File")));
			} catch (Exception e) {
				exportFileName.setText("---");
			}

			try {
				stateLabel.setText(bank.getImportExportState());
			} catch (Exception e) {
				stateLabel.setText("---");
			}

			importFileName.setText(bank.getAttribute("Name of Import File"));

			centerIfOutOfView();
		}
	}

	private String localizeTime(String iso) {
		try {
			DateTimeFormat f = DateTimeFormat.getFormat("yyyy-MM-ddTHH:mm:ssZZZZ");
			Date d = f.parse(iso);
			DateTimeFormat locale = DateTimeFormat.getFormat(PredefinedFormat.DATE_TIME_SHORT);
			return locale.format(d, new GMTTimeZone());
		} catch (Exception e) {
			return iso;
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

	private void setFocus(Widget w) {
		haveFocus = w;
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		theBank = pm.findBank(pm.getSelectedBank());
	}

	public static void update() {
		PresetManager pm = NonMaps.theMaps.getNonLinearWorld().getPresetManager();
		update(pm.findBank(pm.getSelectedBank()));
	}
}