package com.nonlinearlabs.NonMaps.client.world.maps.presets.bank;

import java.util.ArrayList;
import java.util.HashMap;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.Renameable;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.IBank;
import com.nonlinearlabs.NonMaps.client.world.IPreset;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.Label;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.NonDimension;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter.Initiator;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.MultiplePresetSelection;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Tape.Orientation;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.DragProxy;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.EditBufferDraggingButton;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets.PresetList;

public class Bank extends LayoutResizingVertical implements Renameable, IBank {

	private Position dragPosition = null;
	private DropAction currentDropAction;
	private String uuid = "";
	final Header header = new Header(this);
	private boolean minimized = false;
	private String selectedPreset = "";
	private Label emptyLabel = null;
	private PrevNextButtons prevNext;
	private int orderNumber = 0;
	private HashMap<String, String> attributes = new HashMap<String, String>();
	private String dateOfLastChange = "---";

	private Bank masterLeft = null;
	private Bank masterTop = null;
	private Bank slaveRight = null;
	private Bank slaveBottom = null;

	private boolean mouseCaptured = false;
	private String masterUUID;
	private String attatchDirection;

	private String m_saveState = "";

	private enum DropPosition {
		ABOVE, COVERING, BELOW
	}

	public enum DropAction {
		NONE, COPY_PRESET, MOVE_PRESET, INSERT_EDITBUFFER, INSERT_BANK, DROP_PRESETS
	}

	private Tape tapes[] = new Tape[4];

	public Bank(PresetManager parent, String uuid) {
		super(parent);
		this.uuid = uuid;
		addChild(header);
		prevNext = addChild(new PrevNextButtons(this));

		tapes[Tape.Orientation.North.ordinal()] = addChild(new Tape(this, Tape.Orientation.North));
		tapes[Tape.Orientation.South.ordinal()] = addChild(new Tape(this, Tape.Orientation.South));
		tapes[Tape.Orientation.East.ordinal()] = addChild(new Tape(this, Tape.Orientation.East));
		tapes[Tape.Orientation.West.ordinal()] = addChild(new Tape(this, Tape.Orientation.West));
	}

	@Override
	public boolean skipChildOnLayout(MapsControl c) {
		return c instanceof Tape;
	}

	public Tape getTape(Tape.Orientation o) {
		return tapes[o.ordinal()];
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		NonDimension oldDim = getNonPosition().getDimension();

		double tapeWidth = getAttachArea();

		for (MapsControl c : getChildren()) {
			c.getNonPosition().moveBy(tapeWidth, tapeWidth);
		}

		getTape(Tape.Orientation.North).setNonSize(new NonDimension(oldDim.getWidth(), tapeWidth));
		getTape(Tape.Orientation.North).moveTo(new NonPosition(tapeWidth, 0));

		getTape(Tape.Orientation.South).setNonSize(new NonDimension(oldDim.getWidth(), tapeWidth));
		getTape(Tape.Orientation.South).moveTo(new NonPosition(tapeWidth, oldDim.getHeight() + tapeWidth));

		getTape(Tape.Orientation.West).setNonSize(new NonDimension(tapeWidth, oldDim.getHeight()));
		getTape(Tape.Orientation.West).moveTo(new NonPosition(0, tapeWidth));

		getTape(Tape.Orientation.East).setNonSize(new NonDimension(tapeWidth, oldDim.getHeight()));
		getTape(Tape.Orientation.East).moveTo(new NonPosition(oldDim.getWidth() + tapeWidth, tapeWidth));

		setNonSize(oldDim.getWidth() + tapeWidth * 2, oldDim.getHeight() + tapeWidth * 2);
	}

	protected void drawDropIndicator(Context2d ctx) {
		if (dragPosition != null) {
			for (Control c : getChildren()) {
				if (c instanceof IPreset) {
					Rect presetRect = c.getPixRect();

					if (presetRect.contains(dragPosition)) {
						drawDropIndicator(ctx, presetRect);
						return;
					} else if (presetRect.contains(new Position(dragPosition.getX(), dragPosition.getY() + toYPixels(getPadding())))) {
						drawDropIndicator(ctx, presetRect);
						return;
					} else if (presetRect.contains(new Position(dragPosition.getX(), dragPosition.getY() - toYPixels(getPadding())))) {
						drawDropIndicator(ctx, presetRect);
						return;
					}
				}
			}
		}
	}

	public double getAttachArea() {
		return 15;
	}

	private void drawDropIndicator(Context2d ctx, Rect presetRect) {
		ctx.setFillStyle("rgba(255, 0, 0, 0.5)");

		switch (getDropPosition(presetRect, dragPosition)) {
		case ABOVE:
			drawDropIndicator(ctx, presetRect, -0.1, 0.2);
			break;

		case BELOW:
			drawDropIndicator(ctx, presetRect, 0.9, 0.2);
			break;

		case COVERING:
			drawDropIndicator(ctx, presetRect, 0.1, 0.8);
			break;
		}
	}

	private void drawDropIndicator(Context2d ctx, Rect rect, double yOffset, double heightFactor) {
		ctx.fillRect(rect.getLeft(), rect.getTop() + rect.getHeight() * yOffset, rect.getWidth(), rect.getHeight() * heightFactor);
	}

	@Override
	public Control drag(Position pos, DragProxy dragProxy) {
		if (dragProxy.getPixRect().contains(pos)) { // sort out slaves
			if (!isDraggingControl()) {
				currentDropAction = getDropAction(pos, dragProxy);

				if (currentDropAction != DropAction.NONE) {
					dragPosition = pos;
					invalidate(INVALIDATION_FLAG_UI_CHANGED);
					return this;
				}
			}
		}
		return super.drag(pos, dragProxy);
	}

	@Override
	public int getDragRating(Position newPoint, DragProxy dragProxy) {
		if (getPixRect().contains(newPoint))
			return super.getDragRating(newPoint, dragProxy) * 10000;
		return 0;
	}

	@Override
	public void dragLeave() {
		dragPosition = null;
		currentDropAction = DropAction.NONE;
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}

	@Override
	public Control drop(Position pos, DragProxy dragProxy) {
		if (dragPosition != null) {
			for (Control c : getChildren()) {
				if (c instanceof IPreset) {
					Rect presetRect = c.getPixRect();

					if (presetRect.contains(dragPosition)
							|| presetRect.contains(new Position(dragPosition.getX(), dragPosition.getY() - toYPixels(getPadding())))
							|| presetRect.contains(new Position(dragPosition.getX(), dragPosition.getY() + toYPixels(getPadding())))) {
						IPreset preset = (IPreset) c;
						DropAction action = getDropAction(pos, dragProxy);
						DropPosition dropPosition = getDropPosition(presetRect, dragPosition);
						doDropAction(action, dropPosition, preset, dragProxy);
						break;
					}
				}
			}
			dragPosition = null;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return this;
		}

		return null;
	}

	private void doDropAction(DropAction action, DropPosition pos, IPreset preset, DragProxy dragProxy) {
		switch (action) {
		case COPY_PRESET:
			doCopyPreset(pos, preset, dragProxy);
			break;

		case INSERT_EDITBUFFER:
			doInsertEditbuffer(pos, preset, dragProxy);
			break;

		case MOVE_PRESET:
			doMovePreset(pos, preset, dragProxy);
			break;

		case INSERT_BANK:
			doInsertBank(pos, preset, dragProxy);
			break;

		case DROP_PRESETS:
			doDropPresets(pos, preset);

		case NONE:
		default:
			break;
		}
	}

	private void doDropPresets(DropPosition pos, IPreset preset) {
		PresetManager pm = NonMaps.get().getNonLinearWorld().getPresetManager();
		MultiplePresetSelection sel = pm.getMultiSelection();
		String csv = sel.getCSV();

		switch (pos) {
		case ABOVE:
			getNonMaps().getServerProxy().dropPresetsAbove(csv, preset);
			break;

		case BELOW:
			getNonMaps().getServerProxy().dropPresetsBelow(csv, preset);
			break;

		case COVERING:
			getNonMaps().getServerProxy().dropPresetsTo(csv, preset);
			break;
		}
	}

	private void doMovePreset(DropPosition pos, IPreset preset, DragProxy dragProxy) {
		IPreset p = (IPreset) dragProxy.getOrigin();

		switch (pos) {
		case ABOVE:
			getNonMaps().getServerProxy().movePresetAbove(p, preset);
			break;

		case BELOW:
			getNonMaps().getServerProxy().movePresetBelow(p, preset);
			break;

		case COVERING:
			getNonMaps().getServerProxy().movePresetTo(p, preset);
			break;
		}
	}

	private void doInsertEditbuffer(DropPosition pos, IPreset preset, DragProxy dragProxy) {
		switch (pos) {
		case ABOVE:
			getNonMaps().getServerProxy().insertEditBufferAbove(preset);
			break;

		case BELOW:
			getNonMaps().getServerProxy().insertEditBufferBelow(preset);
			break;

		case COVERING:
			getNonMaps().getServerProxy().overwritePresetWithEditBuffer(preset);
			break;
		}
	}

	private void doInsertBank(DropPosition pos, IPreset preset, DragProxy dragProxy) {
		IBank bank = (IBank) dragProxy.getOrigin();

		Bank bBank = (Bank) bank;
		if (bBank.hasSlaves())
			return;

		switch (pos) {
		case ABOVE:
			getNonMaps().getServerProxy().insertBankAbove(bank, preset);
			break;

		case BELOW:
			getNonMaps().getServerProxy().insertBankBelow(bank, preset);
			break;

		case COVERING:
			getNonMaps().getServerProxy().overwritePresetWithBank(bank, preset);
			break;
		}
	}

	private void doCopyPreset(DropPosition pos, IPreset preset, DragProxy dragProxy) {
		IPreset p = (IPreset) dragProxy.getOrigin();

		switch (pos) {
		case ABOVE:
			getNonMaps().getServerProxy().insertPresetCopyAbove(p, preset);
			break;

		case BELOW:
			getNonMaps().getServerProxy().insertPresetCopyBelow(p, preset);
			break;

		case COVERING:
			getNonMaps().getServerProxy().overwritePresetWith(p, preset);
			break;
		}
	}

	private DropPosition getDropPosition(Rect rect, Position pt) {

		double diffFromTop = pt.getY() - rect.getTop();
		if (diffFromTop < rect.getHeight() * 0.25) {
			return DropPosition.ABOVE;
		} else if (diffFromTop > rect.getHeight() * 0.75) {
			return DropPosition.BELOW;
		} else {
			return DropPosition.COVERING;
		}
	}

	@Override
	public <T extends MapsControl> T addChild(T child) {
		T ret = super.addChild(child);

		if (prevNext != null) {
			removeChild(prevNext);
			super.addChild(prevNext);
		}

		return ret;
	}

	@Override
	public PresetManager getParent() {
		return (PresetManager) super.getParent();
	}

	@Override
	public Control click(Position eventPoint) {
		return this;
	}

	public void toggleMinMax() {
		minimized = !minimized;
		requestLayout();
	}

	@Override
	public double getPadding() {
		return isMinimized() ? 0 : 1;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (isDraggingControl() && !isVisibilityForced())
			return;

		Rect r = getPixRect().copy();
		double reduce = toXPixels(getAttachArea());
		r = r.getReducedBy(2 * reduce);
		r.drawRoundedRect(ctx, Rect.ROUNDING_TOP, toXPixels(6), toXPixels(1), getColorBankSelect(), null);
		super.draw(ctx, invalidationMask);
		drawDropIndicator(ctx);
	}

	public void onMouseLost() {
		mouseCaptured = false;
	}

	public void onMouseCaptured() {
		mouseCaptured = true;
	}

	public boolean isMouseCaptured() {
		return mouseCaptured;
	}

	public RGB getColorBankInnerBorder() {
		return new RGB(77, 77, 77);
	}

	@Override
	public double getXMargin() {
		return 2;
	}

	@Override
	public double getYMargin() {
		return 2;
	}

	public RGB getColorBankSelect() {

		if (NonMaps.get().getNonLinearWorld().getViewport().getOverlay().getDragProxyFor(this) != null)
			return new RGB(98, 113, 183);
		else if (isSelected())
			return new RGB(173, 181, 217);

		else
			return new RGB(150, 150, 150);
	}

	public String getUUID() {
		return uuid;
	}

	@Override
	public RGB getColorFont() {
		return new RGB(250, 250, 250);
	}

	@Override
	public RGB getColorObjectContour() {
		if (isSelected())
			return new RGB(255, 255, 255);

		return new RGB(125, 125, 125);
	}

	public boolean isSelected() {
		return uuid.equals(getParent().getSelectedBank());
	}

	public String getCurrentName() {
		return header.getBankName();
	}

	public String getTitleName() {
		return getCurrentName();
	}

	public void setName(String newName) {
		getNonMaps().getServerProxy().renameBank(getUUID(), newName);
	}

	public String getEntityName() {
		return "Bank";
	}

	public String getSelectedPreset() {
		return selectedPreset;
	}

	public void selectBank(boolean userInteraction) {
		getParent().selectBank(getUUID(), userInteraction);
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}

	public boolean isMinimized() {
		return minimized;
	}

	public void selectPreset(String uuid, Initiator initiator) {
		if (selectedPreset.equals(uuid))
			return;

		selectedPreset = uuid;

		if (initiator == Initiator.EXPLICIT_USER_ACTION)
			getNonMaps().getServerProxy().selectPreset(uuid);

		requestLayout();

		if (isSelected()) {
			NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getBelt().getPresetLayout().getBankControl().getPresetList()
					.scheduleAutoScroll(PresetList.ScrollRequest.Smooth);
		}

		getParent().onPresetSelectionChanged(findPreset(selectedPreset));
	}

	void showEmptyLabel(boolean bankEmpty) {
		if (bankEmpty && emptyLabel == null) {
			emptyLabel = addChild(new EmptyLabel(this, this, "- empty -"));
		} else if (!bankEmpty && emptyLabel != null) {
			removeChild(emptyLabel);
			emptyLabel = null;
		}
	}

	public Preset findLoadedPreset() {
		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				Preset p = (Preset) c;
				if (p.isLoaded())
					return p;
			}
		}
		return null;
	}

	public boolean isLast(Preset p) {
		boolean found = false;

		for (Control c : getChildren()) {
			if (c == p) {
				found = true;
			} else if (c instanceof Preset && found)
				return false;
		}

		return found;
	}

	public boolean isFirst(Preset p) {
		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				return c == p;
			}
		}

		return false;
	}

	public boolean doesBankContainLoadedPreset() {
		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				Preset p = (Preset) c;
				if (p.isLoaded())
					return true;
			}
		}
		return false;
	}

	public Preset getPrev(String uuid) {
		Preset last = null;

		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				Preset p = (Preset) c;
				if (p.getUUID().equals(uuid))
					return last;

				last = p;
			}
		}
		return null;
	}

	public Preset getNext(String uuid) {
		boolean found = false;

		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				Preset p = (Preset) c;

				if (found)
					return p;

				if (p.getUUID().equals(uuid))
					found = true;
			}
		}
		return null;
	}

	public void selectPrev(Initiator initiator) {
		Preset prev = getPrev(selectedPreset);

		if (prev != null)
			selectPreset(prev.getUUID(), initiator);
	}

	public void selectNext(Initiator initiator) {
		Preset next = getNext(selectedPreset);

		if (next != null)
			selectPreset(next.getUUID(), initiator);
	}

	public Preset findPreset(String uuid) {
		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				Preset p = (Preset) c;

				if (p.getUUID().equals(uuid))
					return p;
			}
		}
		return null;
	}

	public boolean hasSelectedPreset() {
		return findPreset(selectedPreset) != null;
	}

	public void setOrderNumber(int i) {
		orderNumber = i;
	}

	public int getOrderNumber() {
		return orderNumber;
	}

	protected DropAction getDropAction(Position pos, DragProxy draggedElement) {

		if (getNonMaps().getNonLinearWorld().getViewport().getOverlay().getSetup().getPresetDragDropSetting().isEnabled()) {

			Control origin = draggedElement.getOrigin();

			if (origin instanceof IPreset) {
				IPreset p = (IPreset) origin;

				PresetManager pm = getParent();

				if (pm.hasMultiplePresetSelection() && pm.getMultiSelection().getNumSelectedPresets() > 1)
					return DropAction.DROP_PRESETS;

				Preset targetPreset = findPresetAt(pos);

				if (targetPreset == null) {
					targetPreset = findPresetAt(new Position(pos.getX(), pos.getY() + toYPixels(getPadding())));

					if (targetPreset == null) {
						targetPreset = findPresetAt(new Position(pos.getX(), pos.getY() - toYPixels(getPadding())));
					}
				}

				if (targetPreset != null && targetPreset.getUUID().equals(p.getUUID())) {
					return DropAction.NONE;
				}

				if (findPreset(p.getUUID()) != null) {
					return DropAction.MOVE_PRESET;
				}

				return DropAction.COPY_PRESET;
			}

			if (origin instanceof EditBufferDraggingButton)
				return DropAction.INSERT_EDITBUFFER;

			if (origin instanceof IBank) {
				if (!origin.equals(this)) {
					Bank bBank = (Bank) origin;
					if (bBank.hasSlaves() == false)
						return DropAction.INSERT_BANK;
				}
			}
		}

		return DropAction.NONE;
	}

	private Preset findPresetAt(Position pos) {
		for (Control c : getChildren())
			if (c instanceof Preset)
				if (c.getPixRect().contains(pos))
					return (Preset) c;

		return null;
	}

	public Preset getPreset(int idx) {
		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				if (idx <= 0) {
					return (Preset) c;
				}
				idx--;
			}
		}

		return null;
	}

	public Preset getLast() {
		Control last = null;
		for (Control c : getChildren())
			if (c instanceof Preset)
				last = c;

		return (Preset) last;
	}

	@Override
	public Control doubleClick() {
		return this;
	}

	public void clearFilter() {
		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				Preset p = (Preset) c;
				p.setFilterState(Preset.FilterState.NO_FILTER);
			}
		}
	}

	public int applyFilter(Node bank) {
		int numMatches = 0;

		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				Preset p = (Preset) c;
				p.setFilterState(Preset.FilterState.FILTERED_OUT);
			}
		}

		NodeList banks = bank.getChildNodes();

		for (int i = 0; i < banks.getLength(); i++) {
			Node child = banks.item(i);

			if (child.getNodeName().equals("preset")) {
				String uuid = child.getAttributes().getNamedItem("uuid").getNodeValue();
				Preset p = findPreset(uuid);
				if (p != null) {
					p.setFilterState(Preset.FilterState.FILTER_MATCHES);
					numMatches++;
				}
			}
		}
		return numMatches;
	}

	public boolean isEmpty() {
		for (Control c : getChildren())
			if (c instanceof Preset)
				return false;

		return true;
	}

	public boolean canNext() {
		Preset p = findPreset(selectedPreset);
		if (p != null)
			return !isLast(p);
		return false;
	}

	public boolean canPrev() {
		Preset p = findPreset(selectedPreset);
		if (p != null)
			return !isFirst(p);
		return false;
	}

	public short getPresetCount() {
		short numberOfPresets = 0;
		for (Control c : getChildren()) {
			if (c instanceof Preset) {
				numberOfPresets++;
			}
		}
		return numberOfPresets;
	}

	void updateAttributes(Node node) {
		attributes.clear();

		NodeList children = node.getChildNodes();

		for (int i = 0; i < children.getLength(); i++) {
			Node n = children.item(i);
			String nodesName = n.getNodeName();

			if (nodesName.equals("attribute")) {
				updateAttribute(n);
			}
		}
	}

	private void updateAttribute(Node n) {
		String key = n.getAttributes().getNamedItem("key").getNodeValue();
		String value = ServerProxy.getText(n);
		attributes.put(key, value);
	}

	public String getAttribute(String key) {
		String ret = attributes.get(key);
		if (ret != null)
			return ret;
		return "";
	}

	public void setDateOfLastChange(String d) {
		dateOfLastChange = d;
	}

	public String getDateOfLastChange() {
		return dateOfLastChange;
	}

	public void setAttribute(Initiator initiator, String key, String value) {
		attributes.put(key, value);

		if (initiator == Initiator.EXPLICIT_USER_ACTION) {
			NonMaps.get().getServerProxy().setBankAttribute(this, key, value);
			requestLayout();
		}
	}

	public void undockBank() {
		NonMaps.theMaps.getServerProxy().undockBank(this);
	}

	public Bank getBottomSlave() {
		return slaveBottom;
	}

	public Bank getRightSlave() {
		return slaveRight;
	}

	public void installRelationshipMasterSlave(Bank master, String direction) {

		if (direction.equals("top") || direction.equals("bottom")) {
			masterTop = master;
			masterLeft = null;
			masterTop.addSlave(this, "bottom");
		} else if (direction.equals("left") || direction.equals("right")) {
			masterTop = null;
			masterLeft = master;
			masterLeft.addSlave(this, "right");
		} else {
			return;
		}
	}

	public void addSlave(Bank slave, String direction) {
		if (direction.equals("bottom") || direction.equals("top")) {
			if (slaveBottom == null)
				slaveBottom = slave;
		} else if (direction.equals("right") || direction.equals("left")) {
			if (slaveRight == null)
				slaveRight = slave;
		}
	}

	public Bank getMaster() {
		if (masterLeft != null)
			return masterLeft;
		else if (masterTop != null)
			return masterTop;
		else
			return null;
	}

	public void removeMaster() {
		masterLeft = null;
		masterTop = null;
	}

	public void removeSlave(Bank slave) {
		if (slaveBottom == slave)
			slaveBottom = null;
		else if (slaveRight == slave)
			slaveRight = null;
	}

	public String getDirectionOfMaster() {
		if (masterLeft != null)
			return "left";
		else if (masterTop != null)
			return "top";
		else
			return "";
	}

	public void removeAllSlaves() {
		slaveBottom = null;
		slaveRight = null;
	}

	public void setAttatchedTo(String direction) {
		this.attatchDirection = direction;
	}

	public void setMasterUUID(String masterUUID) {
		this.masterUUID = masterUUID;
	}

	public String getMasterUUID() {
		return this.masterUUID;
	}

	public String getAttatchDirection() {
		return this.attatchDirection;
	}

	public void collectCluster(ArrayList<Bank> ret) {
		ret.add(this);

		if (slaveBottom != null)
			slaveBottom.collectCluster(ret);

		if (slaveRight != null)
			slaveRight.collectCluster(ret);
	}

	public void layoutSlaves() {
		if (slaveBottom != null) {
			NonPosition posYFin = new NonPosition(getNonPosition().getLeft(), getNonPosition().getBottom() + 15);
			slaveBottom.moveTo(posYFin);
			slaveBottom.layoutSlaves();
		}

		if (slaveRight != null) {
			NonPosition posXFin = new NonPosition(getNonPosition().getRight() + 15, getNonPosition().getTop());
			slaveRight.moveTo(posXFin);
			slaveRight.layoutSlaves();
		}
	}

	public Header getHeader() {
		return header;
	}

	public boolean hasSlaves() {
		return getBottomSlave() != null || getRightSlave() != null;
	}

	public void setImportExportState(String state) {
		m_saveState = state;
	}

	public String getImportExportState() {
		return m_saveState;
	}

	public boolean isTapeActive(Orientation orientation) {
		boolean hasMaster = masterLeft != null || masterTop != null;

		switch (orientation) {
		case North:
			return !hasMaster;

		case South:
			return slaveBottom == null;

		case East: {
			Bank walker = this;
			while (walker != null) {
				if (walker.slaveRight != null)
					return false;
				walker = walker.masterTop;
			}
			return true;
		}

		case West:
			return !hasMaster;
		}
		return false;
	}

	public Tape[] getTapes() {
		return tapes;
	}

	public Bank getMasterTop() {
		return masterTop;
	}

	public Bank getMasterLeft() {
		return masterLeft;
	}

	@Override
	public void beingDragged(double xDiff, double yDiff) {
		getParent().resetAttachingTapes();
		super.beingDragged(xDiff, yDiff);
	}

	public boolean isClusteredWith(Bank parent) {
		Bank master = getClusterMaster();
		return master == parent.getClusterMaster();
	}

	public Bank getClusterMaster() {
		Bank master = this;
		while (true) {
			if (master.masterLeft != null)
				master = master.masterLeft;
			else if (master.masterTop != null)
				master = master.masterTop;
			else
				return master;
		}
	}
}