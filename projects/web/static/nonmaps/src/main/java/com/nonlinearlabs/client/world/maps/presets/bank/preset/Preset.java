package com.nonlinearlabs.client.world.maps.presets.bank.preset;

import java.util.HashMap;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.i18n.client.NumberFormat;
import com.google.gwt.xml.client.Node;
import com.google.gwt.xml.client.NodeList;
import com.nonlinearlabs.client.CustomPresetSelector;
import com.nonlinearlabs.client.LoadToPartMode;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.Renameable;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.StoreSelectMode;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.SoundType;
import com.nonlinearlabs.client.dataModel.editBuffer.EditBufferModel.VoiceGroup;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel.BooleanValues;
import com.nonlinearlabs.client.presenters.EditBufferPresenterProvider;
import com.nonlinearlabs.client.useCases.EditBufferUseCases;
import com.nonlinearlabs.client.world.Control;
import com.nonlinearlabs.client.world.Gray;
import com.nonlinearlabs.client.world.IPreset;
import com.nonlinearlabs.client.world.NonLinearWorld;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.RGB;
import com.nonlinearlabs.client.world.Rect;
import com.nonlinearlabs.client.world.RenameDialog;
import com.nonlinearlabs.client.world.Viewport;
import com.nonlinearlabs.client.world.maps.LayoutResizingHorizontal;
import com.nonlinearlabs.client.world.maps.NonPosition;
import com.nonlinearlabs.client.world.maps.NonRect;
import com.nonlinearlabs.client.world.maps.presets.MultiplePresetSelection;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.client.world.overlay.ContextMenu;
import com.nonlinearlabs.client.world.overlay.DragProxy;
import com.nonlinearlabs.client.world.overlay.Overlay;
import com.nonlinearlabs.client.world.overlay.PresetInfoDialog;
import com.nonlinearlabs.client.world.overlay.belt.presets.PresetContextMenu;
import com.nonlinearlabs.client.world.overlay.belt.presets.PresetDeleter;

public class Preset extends LayoutResizingHorizontal implements Renameable, IPreset {
	private String uuid = null;
	private String realName = "";
	private String partIName = "";
	private String partIIName = "";
	private ColorTag tag = null;
	private TypeLabel typeLabel = null;
	private Name name = null;
	private Number number = null;
	private HashMap<String, String> attributes = new HashMap<String, String>();

	private SoundType type = SoundType.Single;

	private static final PresetColorPack loadedColor = new PresetColorPack(new Gray(0), RGB.blue(), new Gray(77));
	private static final PresetColorPack standardColor = new PresetColorPack(new Gray(0), new Gray(25), new Gray(77));
	private static final PresetColorPack renamedColor = new PresetColorPack(new Gray(0), new Gray(77), new Gray(77));
	private static final PresetColorPack selectedColor = new PresetColorPack(new Gray(0), new Gray(77), new Gray(77));
	private static final PresetColorPack multiSelectedColor = new PresetColorPack(new Gray(0), new Gray(90),
			new Gray(77));

	public Preset(Bank parent) {
		super(parent);

		tag = addChild(new ColorTag(this));
		number = addChild(new Number(this, ""));
		name = addChild(new Name(this, ""));
		typeLabel = addChild(new TypeLabel(this, ""));
	}

	@Override
	public Bank getParent() {
		return (Bank) super.getParent();
	}

	public String getPartName(VoiceGroup vg) {
		if (vg == VoiceGroup.I) {
			return this.partIName;
		} else if (vg == VoiceGroup.II) {
			return this.partIIName;
		}
		return "";
	}

	public boolean isDual() {
		return getType() != SoundType.Single;
	}

	@Override
	public RGB getColorFont() {
		boolean selected = isSelected();
		boolean loaded = isLoaded() && !isInStoreSelectMode();
		boolean isOriginPreset = isLoaded() && isInStoreSelectMode();

		if (isInMultiplePresetSelectionMode()) {
			selected = getParent().getParent().getMultiSelection().contains(this);
			loaded = false;
		}

		if (isOriginPreset)
			return new RGB(255, 255, 255);

		if (!selected && !loaded)
			return new RGB(179, 179, 179);

		return super.getColorFont();
	}

	public boolean isInStoreSelectMode() {
		return getParent().getParent().isInStoreSelectMode();
	}

	public void update(int i, Node preset) {
		this.uuid = preset.getAttributes().getNamedItem("uuid").getNodeValue();
		this.realName = preset.getAttributes().getNamedItem("name").getNodeValue();
		String suffixedName = preset.getAttributes().getNamedItem("name-suffixed").getNodeValue();
		this.number.setText(NumberFormat.getFormat("#000").format(i));
		this.name.setText(suffixedName);

		String typeStr = preset.getAttributes().getNamedItem("type").getNodeValue();

		this.partIName = preset.getAttributes().getNamedItem("part-I-name").getNodeValue();
		this.partIIName = preset.getAttributes().getNamedItem("part-II-name").getNodeValue();
		this.type = SoundType.valueOf(typeStr);
		this.typeLabel.updateType(this.type);

		updateAttributes(preset);

		if (isSelected() && getParent().isSelected() && PresetInfoDialog.isShown())
			PresetInfoDialog.update(this);
	}

	public String getDisplayNameWithSuffix() {
		return this.name.getText();
	}

	public int getNumber() {
		return Integer.parseInt(number.getText());
	}

	public String getPaddedNumber() {
		NumberFormat f = NumberFormat.getFormat("000");
		String ret = f.format(getNumber());
		return ret;
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {

		super.doFirstLayoutPass(levelOfDetail);

		if (getParent().isCollapsed()) {
			if (!isSelected()) {
				tag.getNonPosition().getDimension().setHeight(0);
				number.getNonPosition().getDimension().setHeight(0);
				name.getNonPosition().getDimension().setHeight(0);
				typeLabel.getNonPosition().getDimension().setHeight(0);
				getNonPosition().getDimension().setHeight(0);
			}
		}
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		name.setNonSize(parentsWidthFromFirstPass - number.getNonPosition().getWidth() - tag.getNonPosition().getWidth()
				- typeLabel.getNonPosition().getWidth(), name.getNonPosition().getHeight());
		setNonSize(parentsWidthFromFirstPass, Math.ceil(getNonPosition().getHeight()));
	}

	public PresetColorPack getActiveColorPack() {
		boolean selected = isSelected() || isContextMenuActiveOnMe();
		boolean loaded = isLoaded() && !isInStoreSelectMode();
		boolean isOriginPreset = isLoaded() && isInStoreSelectMode();

		if (isInMultiplePresetSelectionMode()) {
			selected = getParent().getParent().getMultiSelection().contains(this);
			loaded = false;
		}

		return selectAppropriateColor(selected, loaded, isOriginPreset);
	}

	private PresetColorPack selectAppropriateColor(boolean selected, boolean loaded, boolean isOriginPreset) {

		PresetColorPack currentPack = standardColor;

		if (loaded || isOriginPreset)
			currentPack = loadedColor;
		else
			currentPack = selected ? selectedColor : standardColor;

		if (isInMultiplePresetSelectionMode()) {
			MultiplePresetSelection mps = getParent().getParent().getMultiSelection();
			if (mps != null) {
				if (mps.contains(this)) {
					currentPack = multiSelectedColor;
				}
			}
		}

		if (RenameDialog.isPresetBeingRenamed(this)) {
			currentPack = renamedColor;
		}

		if (isDraggingControl()) {
			currentPack = new PresetColorPack(currentPack);
			currentPack.fill.brighter(40);
		}
		return currentPack;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {

		PresetColorPack currentPresetColorPack = getActiveColorPack();

		double cp = getConturPixels();
		cp = Math.ceil(cp);
		cp = Math.max(1, cp);

		Rect r = getPixRect().copy();
		r.fill(ctx, currentPresetColorPack.fill);
		r.stroke(ctx, cp, currentPresetColorPack.highlight);
		r.reduceHeightBy(2 * cp);
		r.reduceWidthBy(2 * cp);
		r.stroke(ctx, cp, currentPresetColorPack.contour);

		super.draw(ctx, invalidationMask);
		r.fill(ctx, currentPresetColorPack.overlay);
	}

	public boolean isSelected() {
		CustomPresetSelector sm = NonMaps.get().getNonLinearWorld().getPresetManager().getCustomPresetSelection();
		if (sm instanceof StoreSelectMode)
			return sm.getSelectedPreset() == this;

		if (PresetDeleter.instance != null)
			if (PresetDeleter.instance.isPresetInSelection(this))
				return true;

		return uuid.equals(getParent().getPresetList().getSelectedPreset());
	}

	public boolean isContextMenuActiveOnMe() {
		Overlay o = NonMaps.get().getNonLinearWorld().getViewport().getOverlay();
		ContextMenu c = o.getContextMenu();

		if (c instanceof PresetContextMenu) {
			PresetContextMenu p = (PresetContextMenu) c;
			if (p.getPreset() == this)
				return true;
		}
		return false;
	}

	public boolean isLoaded() {
		PresetManager pm = getParent().getParent();
		CustomPresetSelector sel = pm.getCustomPresetSelection();
		if (sel instanceof StoreSelectMode)
			return this == sel.getOriginalPreset();

		return uuid.equals(getNonMaps().getNonLinearWorld().getParameterEditor().getLoadedPresetUUID());
	}

	public boolean hasCustomPresetSelection() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().hasCustomPresetSelection();
	}

	public CustomPresetSelector getCustomPresetSelection() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().getCustomPresetSelection();
	}

	private boolean wasJustSelected = false;

	private static ChoosePresetPartDialog choosePresetPart = null;

	private Control loadToPartClickBehaviour(LoadToPartMode mode) {
		if (isDual()) {
			choosePresetPart = new ChoosePresetPartDialog(this, EditBufferModel.get().voiceGroup.getValue());
		} else {
			mode.setSelectedPreset(this);
		}

		return this;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		wasJustSelected = false;
		if (NonMaps.get().getNonLinearWorld().isShiftDown() && !isInMultiplePresetSelectionMode()) {
			getParent().getParent().startMultiSelection(this, true);
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			wasJustSelected = true;
		}

		if (!isInMultiplePresetSelectionMode() && !isSelected()) {
			if (isDraggedWhenPresetDragDropNotEnabled == false) {
				selectPreset();
				wasJustSelected = true;
			}
		}

		getParent().getParent().pushBankOntoTop(getParent());
		isDraggedWhenPresetDragDropNotEnabled = false;
		return this;
	}

	@Override
	public Control click(Position point) {
		Control ret = clickBehaviour();
		wasJustSelected = false;
		return ret;
	}

	private boolean isInLoadToPartMode() {
		CustomPresetSelector cps = getCustomPresetSelection();
		return cps != null && cps instanceof LoadToPartMode;
	}

	private Control clickBehaviour() {
		if (isDraggingControl())
			return this;

		if (isInMultiplePresetSelectionMode()) {
			if (!wasJustSelected) {
				getParent().getParent().getMultiSelection().toggle(this);
				invalidate(INVALIDATION_FLAG_UI_CHANGED);
			}
		} else if (isInLoadToPartMode()) {
			loadToPartClickBehaviour((LoadToPartMode) getCustomPresetSelection());
		} else if (isInStoreSelectMode() || !isSelected()) {
			selectPreset();
		} else if (isSelected() && !wasJustSelected) {
			load();
		}
		return this;
	}

	@Override
	public Control onContextMenu(Position pos) {
		if (isInStoreSelectMode())
			return null;

		boolean showContextMenus = SetupModel.get().localSettings.contextMenus.getValue() == BooleanValues.on;

		if (showContextMenus) {
			Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();

			boolean isInMultiSel = isSelectedInMultiplePresetSelectionMode();

			if (isInMultiSel || (!isInMultiSel && !isInMultiplePresetSelectionMode()))
				return o.setContextMenu(pos, new PresetContextMenu(o, this));
		}
		return this;
	}

	public void selectPreset() {
		CustomPresetSelector selector = getNonMaps().getNonLinearWorld().getPresetManager().getCustomPresetSelection();

		if (selector != null) {
			selector.setSelectedPreset(this);
		} else {
			getParent().getPresetList().selectPreset(getUUID(), true);

		}
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
	}

	private boolean isSelectedInMultiplePresetSelectionMode() {
		MultiplePresetSelection mp = getParent().getParent().getMultiSelection();
		if (mp != null) {
			return mp.getSelectedPresets().contains(this.getUUID());
		}
		return false;
	}

	private boolean isInMultiplePresetSelectionMode() {
		return getParent().getParent().hasMultiplePresetSelection();
	}

	private boolean isDraggedWhenPresetDragDropNotEnabled = false;

	@Override
	public Control startDragging(Position pos) {
		if (SetupModel.get().localSettings.presetDragDrop.getValue() == BooleanValues.on) {
			if (isInMultiplePresetSelectionMode()) {
				return startMultipleSelectionDrag(pos);
			}

			return getNonMaps().getNonLinearWorld().getViewport().getOverlay().createDragProxy(this);
		}

		isDraggedWhenPresetDragDropNotEnabled = true;
		return null;
	}

	public Control startMultipleSelectionDrag(Position mousePos) {
		NonLinearWorld world = getNonMaps().getNonLinearWorld();
		PresetManager pm = world.getPresetManager();
		MultiplePresetSelection selection = pm.getMultiSelection();
		selection.add(this);

		Control ret = null;
		double yMargin = getPixRect().getTop() - mousePos.getY();
		double xMargin = getPixRect().getLeft() - mousePos.getX();

		for (String uuid : selection.getSelectedPresets()) {
			Preset p = pm.findPreset(uuid);
			if (p != null) {
				DragProxy a = world.getViewport().getOverlay().addDragProxy(p);
				if (p == this)
					ret = a;

				double xDiff = mousePos.getX() - p.getPixRect().getLeft();
				double yDiff = mousePos.getY() - p.getPixRect().getTop();
				a.animatePositionOffset(xDiff + xMargin, yDiff + yMargin);
				yMargin += getPixRect().getHeight();
			}
		}

		return ret;
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		if (SetupModel.get().localSettings.presetDragDrop.getValue() == BooleanValues.on) {
			return this;
		} else {
			return getNonMaps().getNonLinearWorld().mouseDrag(oldPoint, newPoint, fine);
		}
	}

	@Override
	public String getCurrentName() {
		return realName;
	}

	@Override
	public String getEntityName() {
		return "Preset";
	}

	@Override
	public void setName(String newName) {
		getNonMaps().getServerProxy().renamePreset(getUUID(), newName);
	}

	@Override
	public String getUUID() {
		return uuid;
	}

	public SoundType getType() {
		return type;
	}

	@Override
	public RGB getColorObjectBackgroundSelected() {
		return new RGB(50, 50, 50);
	}

	@Override
	public double getXMargin() {
		return 3;
	}

	@Override
	public double getTopMargin() {
		return 1;
	}

	public void load() {
		VoiceGroup vg = EditBufferPresenterProvider.getPresenter().voiceGroupEnum;
		LoadToPartMode loadToPart = getParent().getParent().getLoadToPartMode();
		if (loadToPart != null) {
			EditBufferUseCases.get().loadPresetPartIntoPart(loadToPart.getSelectedPreset().getUUID(),
					type != SoundType.Single ? loadToPart.getSelectedPart() : VoiceGroup.I, vg);
		} else {
			EditBufferUseCases.get().loadPreset(getUUID());
		}
	}

	private void updateAttributes(Node node) {
		if (ServerProxy.didChange(node)) {
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

	@Override
	public String getTitleName() {
		return getCurrentName();
	}

	@Override
	public void beingDropped() {
		super.beingDropped();

		if (isInMultiplePresetSelectionMode())
			getParent().getParent().closeMultiSelection();
	}

	public void rename() {
		RenameDialog.open(this);
	}

	public void scrollToCenter() {
		NonLinearWorld world = NonMaps.get().getNonLinearWorld();
		Viewport vp = world.getViewport();
		NonRect vpRect = vp.getNonPosition();
		NonPosition myCenter = getNonPositionRelativeToParent(world.getPresetManager()).getCenterPoint();
		NonRect newVpRect = new NonRect(myCenter.getX() - vpRect.getWidth() / 2,
				myCenter.getY() - vpRect.getHeight() / 2,
				vpRect.getWidth(), vpRect.getHeight());
		world.animateViewport(newVpRect, true);
	}

}
