package com.nonlinearlabs.client;

import com.google.gwt.http.client.URL;
import com.nonlinearlabs.client.dataModel.editBuffer.ParameterId;

class StaticURI implements RemoteProcedureCall {

	public static class KeyValue {
		private String key;
		private String value;

		public KeyValue(String key, String value) {
			this.key = key;
			this.value = value;
		}

		public KeyValue(String key, double d) {
			this(key, Double.toString(d));
		}

		public KeyValue(String key, boolean d) {
			this(key, d ? "1" : "0");
		}

		public KeyValue(String key, ParameterId d) {
			this(key, d.toString());
		}

		@Override
		public String toString() {
			return URL.encodeQueryString(key) + "=" + URL.encodeQueryString(value);
		}
	}

	public static class Path {
		private String path;

		public Path(String... components) {
			path = concat(components);
		}

		private String concat(String[] uriComponents) {
			StringBuilder builder = new StringBuilder();

			for (String s : uriComponents) {
				builder.append("/");
				builder.append(s);
			}

			return builder.toString();
		}

		@Override
		public String toString() {
			return path;
		}
	}

	private Path path;
	private KeyValue[] params;

	public StaticURI(Path path, KeyValue... params) {
		this.path = path;
		this.params = params;
	}

	private String concat(KeyValue[] params, boolean isOracle) {
		StringBuilder builder = new StringBuilder();

		for (KeyValue s : params)
			addKeyValue(builder, s, "?", "&");

		addKeyValue(builder, new KeyValue("isOracle", isOracle), "?", "&");
		return builder.toString();
	}

	private void addKeyValue(StringBuilder builder, KeyValue s, String startDelim, String delim) {
		boolean isEmpty = builder.length() == 0;
		builder.append(isEmpty ? startDelim : delim);
		builder.append(s);
	}

	@Override
	public String getURI(boolean isOracle) {
		return path.toString() + concat(params, isOracle);
	}

	public String getPostData(boolean isOracle) {
		StringBuilder builder = new StringBuilder();

		for (KeyValue s : params)
			addKeyValue(builder, s, "", "&");

		addKeyValue(builder, new KeyValue("isOracle", isOracle), "", "&");
		return builder.toString();
	}

}