package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class PresetColorTagContextMenu extends ContextMenu {
	
	private RGB getColor(String color) {
		switch(color) {
			case "green":
				return new RGB(0, 255, 0);
			case "blue":
				return new RGB(0, 0, 255);
			case "yellow":
				return new RGB(255, 255, 0);
			case "orange":
				return new RGB(255, 126, 0);
			case "purple":
				return new RGB(255, 0, 255);
			case "red":
				return new RGB(255, 0, 0);
			default:
				return new RGB(255, 255, 255);
		}
	}
	
	private final Preset preset;	
	
	
	private void setColor(String color) {
		final PresetManager pm = preset.getParent().getParent();
		final boolean hasMultipleSelection = pm.hasMultiplePresetSelection();
		
		if(hasMultipleSelection) {
			for(String uuid: pm.getMultiSelection().getSelectedPresets())
			{
				Preset p = pm.findPreset(uuid);
				NonMaps.get().getServerProxy().setPresetAttribute(p, "color", color);
			}
		}
		else {
			NonMaps.get().getServerProxy().setPresetAttribute(preset, "color", color);
		}
	}
	
	public PresetColorTagContextMenu(OverlayLayout parent, final Preset p) {
		super(parent);
		this.preset = p;

		
		
		
		

		addChild(new ContextMenuItem((ContextMenu)this, "Green") {
			@Override
			public Control click(Position eventPoint) {
				setColor("green");
				return this;
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Blue") {
			@Override
			public Control click(Position eventPoint) {
				setColor("blue");
				return this;
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Yellow") {
			@Override
			public Control click(Position eventPoint) {
				setColor("yellow");
				return this;
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Orange") {
			@Override
			public Control click(Position eventPoint) {
				setColor("orange");
				return this;
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Purple") {
			@Override
			public Control click(Position eventPoint) {
				setColor("purple");
				return this;
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Red") {
			@Override
			public Control click(Position eventPoint) {
				setColor("red");
				return this;
			}
		});
		
	}

}
