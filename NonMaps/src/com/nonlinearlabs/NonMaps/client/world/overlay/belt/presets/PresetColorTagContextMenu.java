package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

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
				if(p.getAttribute("color") == color)
					NonMaps.get().getServerProxy().setPresetAttribute(p, "color", "none");
				else
					NonMaps.get().getServerProxy().setPresetAttribute(p, "color", color);
			}
		}
		else {
			if(preset.getAttribute("color") == color)
				NonMaps.get().getServerProxy().setPresetAttribute(preset, "color", "none");
			else
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
			
			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);
				RGB c = getColor("green");

				ctx.setFillStyle(c.toString());
				ctx.fillRect(0, 0, 1, 4);
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Blue") {
			@Override
			public Control click(Position eventPoint) {
				setColor("blue");
				return this;
			}
			
			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);
				RGB c = getColor("blue");

				ctx.setFillStyle(c.toString());
				ctx.fillRect(0, 0, 1, 4);
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Yellow") {
			@Override
			public Control click(Position eventPoint) {
				setColor("yellow");
				return this;
			}
			
			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);
				RGB c = getColor("yellow");

				ctx.setFillStyle(c.toString());
				ctx.fillRect(0, 0, 1, 4);
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Orange") {
			@Override
			public Control click(Position eventPoint) {
				setColor("orange");
				return this;
			}
			
			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);
				RGB c = getColor("orange");

				ctx.setFillStyle(c.toString());
				ctx.fillRect(0, 0, 1, 4);
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Purple") {
			@Override
			public Control click(Position eventPoint) {
				setColor("purple");
				return this;
			}
			
			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);
				RGB c = getColor("purple");

				ctx.setFillStyle(c.toString());
				ctx.fillRect(0, 0, 1, 4);
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Red") {
			@Override
			public Control click(Position eventPoint) {
				setColor("red");
				return this;
			}
			
			@Override
			public void draw(Context2d ctx, int invalidationMask) {
				super.draw(ctx, invalidationMask);
				RGB c = getColor("red");

				ctx.setFillStyle(c.toString());
				ctx.fillRect(0, 0, 1, 4);
			}
		});
		
		addChild(new ContextMenuItem((ContextMenu)this, "Remove") {
			@Override
			public Control click(Position eventPoint) {
				setColor("");
				return this;
			}
		});
		
	}

}
