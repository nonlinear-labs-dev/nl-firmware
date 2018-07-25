package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import java.util.ArrayList;
import java.util.List;

import com.nonlinearlabs.NonMaps.client.ClipboardManager.ClipboardContent;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RenameDialog;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.CompareDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.PresetContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.PresetInfoDialog;

public class PresetContextMenu extends ContextMenu {

	private Preset preset;

	public PresetContextMenu(OverlayLayout parent, final Preset preset) {
		super(parent);
		this.preset = preset;
		final PresetManager pm = preset.getParent().getParent();
		final boolean hasMultipleSelection = pm.hasMultiplePresetSelection();
		final boolean multipleSelectionAllowed = getNonMaps().getNonLinearWorld().getViewport().getOverlay().getSetup()
				.getPresetDragDropSetting().isEnabled();

		if (multipleSelectionAllowed) {
			addChild(new ContextMenuItem(this, pm.hasMultiplePresetSelection() ? "Finish Multiple Selection" : "Start Multiple Selection") {
				@Override
				public Control click(Position eventPoint) {
					if (pm.hasMultiplePresetSelection())
						pm.closeMultiSelection();
					else
						pm.startMultiSelection(preset, false);
					return super.click(eventPoint);
				}
			});
		}

		if (!hasMultipleSelection) {
			if (!PresetInfoDialog.isShown()) {
				String presetInfoText = "Preset Info ...";
				addChild(new PresetContextMenuItem(this, presetInfoText) {
					@Override
					public Control click(Position eventPoint) {
						preset.selectPreset();
						PresetInfoDialog.toggle();
						invalidate(INVALIDATION_FLAG_UI_CHANGED);
						return super.click(eventPoint);
					}
				});
			}

			addChild(new PresetContextMenuItem(this, "Rename ...") {
				@Override
				public Control click(Position eventPoint) {
					RenameDialog.open(preset);
					return super.click(eventPoint);
				}
			});

			addChild(new PresetContextMenuItem(this, "Cut") {
				@Override
				public Control click(Position eventPoint) {
					getNonMaps().getServerProxy().cutPreset(preset);
					return super.click(eventPoint);
				}
			});

			addChild(new PresetContextMenuItem(this, "Copy") {
				@Override
				public Control click(Position eventPoint) {
					getNonMaps().getServerProxy().copyPreset(preset);				
					return super.click(eventPoint);
				}
			});

			if (getNonMaps().getNonLinearWorld().getClipboardManager().getClipboardState() != ClipboardContent.empty) {
				addChild(new PresetContextMenuItem(this, "Paste") {
					@Override
					public Control click(Position eventPoint) {
						getNonMaps().getServerProxy().pasteOnPreset(preset);						
						return super.click(eventPoint);
					}
				});
			}
		}
		else {
			addChild(new PresetContextMenuItem(this, "Copy") {
				@Override
				public Control click(Position eventPoint) {
					getNonMaps().getServerProxy().copyPresets(pm.getMultiSelection().getCSV());				
					return super.click(eventPoint);
				}
			});
		}

		addChild(new PresetContextMenuItem(this, "Delete") {
			@Override
			public Control click(Position eventPoint) {

				if (hasMultipleSelection) {
					String csv = pm.getMultiSelection().getCSV();
					if(pm.getMultiSelection().selectionContainsSolePresets()) {
						PresetDeleter.open(csv);
					} else {
						NonMaps.get().getServerProxy().deletePresets(csv, false);
					}
				} else {
					if(preset.getParent().getPresetList().getPresetCount() == 1)
						PresetDeleter.open(preset);
					else
						NonMaps.get().getServerProxy().deletePreset(preset, false);
				}
				return super.click(eventPoint);
			}
		});

		if (hasMultipleSelection && pm.getMultiSelection().getNumSelectedPresets() == 2) {
			addChild(new PresetContextMenuItem(this, "Compare ...") {
				@Override
				public Control click(Position eventPoint) {

					ArrayList<String> selPresets = pm.getMultiSelection().getSelectedPresets();
					Preset p1 = pm.findPreset(selPresets.get(0));
					Preset p2 = pm.findPreset(selPresets.get(1));
					CompareDialog.open(p1, p2);
										
					return super.click(eventPoint);
				}
			});
		} else if (hasMultipleSelection == false) {
			addChild(new PresetContextMenuItem(this, "Compare to Editbuffer ...") {
				@Override
				public Control click(Position eventPoint) {
					CompareDialog.open(preset);					
					return super.click(eventPoint);
				}
			});
		}

		addChild(new ContextMenuItem(this, "Color Tag >") {
			@Override
			public Control click(Position eventPosition) {
				Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
				return o.setContextMenu(this.getPixRect().getPosition(), new PresetColorTagContextMenu(o, preset));
			}
		});
	}

	public Preset getPreset() {
		return preset;
	}

}
