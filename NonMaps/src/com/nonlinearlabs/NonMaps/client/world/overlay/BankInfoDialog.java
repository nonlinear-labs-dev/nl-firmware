package com.nonlinearlabs.NonMaps.client.world.overlay;

import java.util.Date;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.event.dom.client.BlurEvent;
import com.google.gwt.event.dom.client.BlurHandler;
import com.google.gwt.event.dom.client.ChangeEvent;
import com.google.gwt.event.dom.client.ChangeHandler;
import com.google.gwt.event.dom.client.DragEndEvent;
import com.google.gwt.event.dom.client.DragEndHandler;
import com.google.gwt.event.dom.client.FocusEvent;
import com.google.gwt.event.dom.client.FocusHandler;
import com.google.gwt.event.dom.client.GestureEndEvent;
import com.google.gwt.event.dom.client.GestureEndHandler;
import com.google.gwt.event.dom.client.KeyCodes;
import com.google.gwt.event.dom.client.KeyPressEvent;
import com.google.gwt.event.dom.client.KeyPressHandler;
import com.google.gwt.event.dom.client.MouseDownEvent;
import com.google.gwt.event.dom.client.MouseDownHandler;
import com.google.gwt.event.dom.client.MouseUpEvent;
import com.google.gwt.event.dom.client.MouseUpHandler;
import com.google.gwt.event.logical.shared.ResizeEvent;
import com.google.gwt.event.logical.shared.ResizeHandler;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.i18n.client.DateTimeFormat;
import com.google.gwt.i18n.client.DateTimeFormat.PredefinedFormat;
import com.google.gwt.user.client.Event;
import com.google.gwt.user.client.Event.NativePreviewEvent;
import com.google.gwt.user.client.Event.NativePreviewHandler;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.TextArea;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.Widget;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;

public class BankInfoDialog extends GWTDialog {

	private static BankInfoDialog theDialog;

	private static float commentBoxHeight = 0;
	
	private TextBox name;
	private TextBox position;
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
		
		if(commentBoxHeight != 0)
		{
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
		FlexTable panel = new FlexTable();
		addRow(panel, "Name", name = new TextBox());
		addRow(panel, "Position", position = new TextBox());
		addRow(panel, "Comment", comment = new TextArea());
		addRow(panel, "Size", size = new Label(""));
		addRow(panel, "State", stateLabel = new Label(""));
		addRow(panel, "Last Change", lastChange = new Label(""));
		addRow(panel, "Import Date", importFileDate = new Label(""));
		addRow(panel, "Import Name", importFileName = new Label(""));
		addRow(panel, "Export Date", exportFileDate = new Label(""));
		addRow(panel, "Export Name", exportFileName = new Label(""));

		comment.addFocusHandler(new FocusHandler() {

			@Override
			public void onFocus(FocusEvent event) {
				setFocus(comment);
			}
		});

		comment.addMouseDownHandler(new MouseDownHandler() 
		{
			private HandlerRegistration mouseMoveUpRegistration;
			private int lastWidth;
			private int lastHeight;

			@Override
			public void onMouseDown(MouseDownEvent event) 
			{
				lastWidth = getOffsetWidth();
				lastHeight = getOffsetHeight();
	
				if (mouseMoveUpRegistration == null) 
				{
					mouseMoveUpRegistration = Event.addNativePreviewHandler(new NativePreviewHandler() 
					{
						@Override
						public void onPreviewNativeEvent(NativePreviewEvent event) 
						{
							if (event.getTypeInt() == Event.ONMOUSEMOVE || event.getTypeInt() == Event.ONMOUSEUP) 
							{
								int width = getOffsetWidth();
								int height = getOffsetHeight();
								if (width != lastWidth || height != lastHeight) 
								{
									commentBoxHeight = comment.getElement().getClientHeight();
									
									lastWidth = width;
									lastHeight = height;
								}
	
								if (event.getTypeInt() == Event.ONMOUSEUP) 
								{
									commentBoxHeight = comment.getElement().getClientHeight();
									
									if (mouseMoveUpRegistration != null) 
									{
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
					position.setFocus(true);
				}
			}
		});

		position.addKeyPressHandler(new KeyPressHandler() {
			
			@Override
			public void onKeyPress(KeyPressEvent event) {
				if (event.getCharCode() == KeyCodes.KEY_ENTER) {
					position.setFocus(false);
					comment.setFocus(true);
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
					int currentValue = new Integer(position.getValue());

					if (oldNumber != currentValue) {
						NonMaps.get().getServerProxy().setBankOrderNumber(theBank, currentValue);
					}
					
					updateInfo(theBank);
				}
			}
		});
		
		setWidget(panel);
		setFocus(panel);
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
			if (!NonMaps.theMaps.getNonLinearWorld().getPresetManager().isEmpty())
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

	private void updateInfo(Bank bank) {
		if (bank != null) {
			String bankName = bank.getCurrentName();
			String commentText = bank.getAttribute("Comment");
			int bankPos = bank.getOrderNumber();

			if (haveFocus != comment) {
				if (!commentText.equals(comment.getText())) {
					comment.setText(commentText);
				}
			}

			if (haveFocus != name) {
				if (!bankName.equals(name.getText())) {
					name.setText(bankName);
				}
			}
			
			int currentPositionValue = 0;
			try {
				currentPositionValue = new Integer(position.getValue());
			} catch(Exception e) {};
			
			if(haveFocus != position) {
				if(bankPos != currentPositionValue) {
					position.setText(new Integer(bankPos).toString());
				}
			}
			
			size.setText(Integer.toString(bank.getPresetCount()));

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
			return locale.format(d);
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