package com.nonlinearlabs.NonMaps.client.world.maps.settings;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.dataModel.setup.Setup;
import com.nonlinearlabs.NonMaps.client.useCases.LocalSettings;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class DeveloperSettings extends LayoutResizingVertical {

	public DeveloperSettings(MapsLayout parent) {
		super(parent);

		addChild(new SetupHeader(this));
		addChild(new SendPresetAsLPCWriteFallback(this));
		addChild(new ResetEditBuffer(this));
		addChild(new SyncLPCToBBB(this));
		addChild(new DebugLevel(this));
		addChild(new UploadPreset(this));
		addChild(new DownloadEditBufferAsText(this));
		addChild(new DownloadErrorReport(this));
		addChild(new OpenEditBufferAsText(this));
		addChild(new TestHWUIDisplay(this));
		addChild(new KioskMode(this));
		addChild(new BlockingMainThreadIndication(this));
		addChild(new BenderRampBypassSetting(this));

		Setup.get().localSettings.showDeveloperOptions.onChange(v -> {
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
			return true;
		});
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (Setup.get().localSettings.showDeveloperOptions.isTrue())
			super.draw(ctx, invalidationMask);
	}

	public String get(String key, String def) {
		return LocalSettings.get().getStorage().load(key, def);
	}

	public void set(String key, String val) {
		LocalSettings.get().getStorage().store(key, val);
	}

	public boolean isOneOf(String key, String... choices) {
		String v = get(key, "");

		for (String c : choices)
			if (c.equals(v))
				return true;

		return false;
	}

	public void setToDefaults() {
		for (MapsControl c : getChildren()) {
			if (c instanceof Setting)
				((Setting) c).setDefault();
		}

	}
}
