package com.nonlinearlabs.NonMaps.client.world.maps;

import java.util.HashMap;

import com.google.gwt.xml.client.Document;
import com.google.gwt.xml.client.XMLParser;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.ServerProxy.DownloadHandler;

public class SVGImageStore {

	static private SVGImageStore self = new SVGImageStore();

	static public SVGImageStore get() {
		return self;
	}

	private HashMap<String, Document> sources = new HashMap<String, Document>();

	public interface SVGDownloadHandler {
		void onSVGDownloaded(Document doc);
	}

	public void loadImage(final String name, final SVGDownloadHandler r) {
		Document doc = sources.get(name);

		if (doc == null) {
			NonMaps.theMaps.getServerProxy().downloadFile(name, new DownloadHandler() {

				@Override
				public void onFileDownloaded(String text) {
					Document dom = XMLParser.parse(text);
					sources.put(name, dom);

					if (r != null)
						r.onSVGDownloaded(dom);
				}

				@Override
				public void onError() {
				}
			});
		} else {
			if (r != null)
				r.onSVGDownloaded(doc);
		}
	}

}
