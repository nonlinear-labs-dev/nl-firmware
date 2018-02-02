package com.nonlinearlabs.NonMaps.client.world.overlay;

import java.util.Arrays;
import java.util.Formatter;
import java.util.List;

import com.google.gwt.core.shared.GWT;
import com.google.gwt.dom.client.Style.Unit;
import com.google.gwt.layout.client.Layout.Alignment;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.LayoutPanel;
import com.google.gwt.user.client.ui.RootPanel;
import com.google.gwt.user.client.ui.ScrollPanel;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy.DownloadHandler;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;

public class CompareDialog extends GWTDialog {

	
	Label leftPreset, rightPreset, parameterCaption;
	Preset preset1, preset2;
	String csvWithDiffs = "";
	FlexTable table = null;
	Label header[] = null;
	ScrollPanel scrollContent = null;
	LayoutPanel panel = null;
	
	static CompareDialog theDialog = null;
	
	public static void open(Preset p1)
	{
		theDialog = new CompareDialog(p1);
	}
	
	public static void open(Preset p1, Preset p2)
	{
		theDialog = new CompareDialog(p1, p2);
	}
	
	private CompareDialog(Preset p1) {
		preset1 = p1;
		preset2 = null;
		init();
	}
	
	private CompareDialog(Preset p1, Preset p2) {
		preset1 = p1;
		preset2 = p2;
		init();
	}
	
	private void init() {
		RootPanel.get().add(this);

		getElement().addClassName("preset-compare-dialog");

		initalShow();

		setAnimationEnabled(true);
		setGlassEnabled(false);
		setModal(false);
		setWidth("25em");

		addHeader("Preset Comparison");
		
		
		if(preset2 == null)
			getCsvOfEditBuffer();
		else
			getCsv();
		
	}
		
	private void addRow(FlexTable panel, String groupName, String paraName,String value1,String value2)
	{
		padd(value1);
		padd(value2);
		int c = panel.getRowCount();
		panel.setWidget(c, 0, new Label(groupName + " - " + paraName));
		panel.setWidget(c, 1, new Label(value1));
		panel.setWidget(c, 2, new Label(value2));
	}
	
	private void padd(String v1)
	{
		String padded = new String(new char[30 - v1.length()]).replace('\0', ' ') + v1;
		v1 = padded;
	}
	
	private void addContent() {
		clear();
		
		header = new Label[4];
		
		for(int i = 0; i < 4; i ++)
		{
			header[i] = new Label();
		}
		
		header[0].setText("Group");
		header[1].setText("Parameter");
		header[2].setText(preset1.getTitleName() + " - " + preset1.getParent().getTitleName());
		if(preset2 != null)
			header[3].setText(preset2.getTitleName() + " - " + preset2.getParent().getTitleName());
		else
			header[3].setText("Editbuffer");

		panel = new LayoutPanel();
		panel.setWidth("600px");
		panel.setHeight("500px");
		
		
		table = new FlexTable();
		table.setWidth("100%");
		
		printCsv(table);
		
		scrollContent = new ScrollPanel(table);
		scrollContent.setHeight("450px");
		scrollContent.setWidth("100%");
				
		for(Label l: header)
		{
			l.setHeight("50px");
			l.setWidth("130px");
			panel.add(l);
			panel.setWidgetTopHeight(l, 0, Unit.PX, 50, Unit.PX);
		}
		panel.setWidgetLeftRight(header[0], 5, Unit.PX, 0, Unit.PX);
		panel.setWidgetLeftRight(header[1], 100, Unit.PX, 0, Unit.PX);
		panel.setWidgetLeftRight(header[2], 300, Unit.PX, 0, Unit.PX);
		panel.setWidgetLeftRight(header[3], 420, Unit.PX, 0, Unit.PX);
		
		panel.add(scrollContent);
		panel.setWidgetBottomHeight(scrollContent, 0, Unit.PX, 450, Unit.PX);
		
		add(panel);
	}
	
	private void getCsv()
	{
		NonMaps.theMaps.getServerProxy().getDifferencesOf2PresetsAsCsv(preset1.getUUID(), preset2.getUUID(), new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				csvWithDiffs = text;
				GWT.log(csvWithDiffs);
				addContent();
				
			}
			@Override
			public void onError() {
				csvWithDiffs = "";
				GWT.log("no response recieved!");
			}
		});
	}
	
	private void getCsvOfEditBuffer() {
		NonMaps.theMaps.getServerProxy().getDifferencesOfPresetsToEditbufferAsCsv(preset1.getUUID(), new DownloadHandler() {
			@Override
			public void onFileDownloaded(String text) {
				csvWithDiffs = text;
				GWT.log(csvWithDiffs);
				addContent();
				
			}
			@Override
			public void onError() {
				csvWithDiffs = "";
				GWT.log("no response recieved!");
			}
		});
	}
	
	private void printCsv(FlexTable table)
	{
		List<String> splitted = Arrays.asList(csvWithDiffs.split(","));
	    		
		for(String entry: splitted)
		{
			GWT.log("array entry: " +entry);
		}
		
		for(int i = 0; i+3 < splitted.size(); i += 4)
		{
			addRow(table, splitted.get(i), splitted.get(i+1), splitted.get(i+2), splitted.get(i+3));
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

	
	
	@Override
	protected void commit() {
		hide();
		theDialog = null;
		NonMaps.theMaps.captureFocus();
		NonMaps.theMaps.getNonLinearWorld().requestLayout();
	}

}
