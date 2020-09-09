package com.nonlinearlabs.client.world.overlay.layouter;

import java.util.ArrayList;

public class HarmonicLayouter {

	public HarmonicLayouter() {
	}

	public class Result {
		public Record record;
		public double width;
	}

	public class Record {
		public Object attached;
		int hideGroup;
		double minWidth;
		double maxWidth;
		int prio;
	}

	public void push(Object attached, double minWidth, double maxWidth, int hideGroup, int prio) {
		Record r = new Record();
		r.attached = attached;
		r.hideGroup = hideGroup;
		r.minWidth = minWidth;
		r.maxWidth = maxWidth;
		r.prio = prio;
		records.add(r);
	}

	public Result[] layout(final double availableWidth) {
		int hideGroup = 0;
		Result[] result = createResultArray();

		for (int i = 0; i < 100; i++) {
			double actualWidth = getActualWidth(result);
			double diff = actualWidth - availableWidth;

			if (diff < 0) {
				result[0].width += (-diff / 2);
				break;
			}

			double minWidth = getMinWidth(result);

			if (minWidth > availableWidth) {
				if (!hideGroup(result, hideGroup++))
					break;

				continue;
			}

			diff = minimizeDiff(result, diff);

			if (Math.round(diff) == 0)
				break;

		}
		return result;
	}

	private boolean hideGroup(Result[] result, int i) {
		boolean ret = false;

		for (Result r : result) {
			if (r.record.hideGroup == i) {
				r.width = 0;
				ret = true;
			}
		}

		return ret;
	}

	private Result[] createResultArray() {
		Result result[] = new Result[records.size()];

		for (int i = 0; i < records.size(); i++) {
			result[i] = new Result();
			result[i].record = records.get(i);
			result[i].width = result[i].record.maxWidth;
		}
		return result;
	}

	private double minimizeDiff(Result[] result, double diff) {
		for (int i = 0; i < 100 && diff != 0; i++) {

			double newDiff = resize(result, diff);

			if (newDiff == diff) {
				diff = newDiff;
				break;
			}

			diff = newDiff;
		}
		return diff;
	}

	private double resize(Result[] result, final double diff) {
		double ret = diff;

		double prios = 0;

		for (Result r : result) {
			if (r.width > 0 && r.width > r.record.minWidth) {
				prios += r.record.prio;
			}
		}

		for (Result r : result) {
			if (r.width > 0 && r.width > r.record.minWidth) {
				double prio = r.record.prio / prios;
				double desiredDiff = diff * prio;
				double maxDiff = r.width - r.record.minWidth;
				double applyDiff = Math.min(maxDiff, desiredDiff);
				r.width -= applyDiff;
				ret -= applyDiff;
			}
		}
		return ret;
	}

	private double getActualWidth(Result[] result) {
		double actualWidth = 0;

		for (Result r : result)
			actualWidth += r.width;
		return actualWidth;
	}

	private double getMinWidth(Result[] result) {
		double actualWidth = 0;

		for (Result r : result) {
			if (r.width != 0)
				actualWidth += r.record.minWidth;
		}
		return actualWidth;
	}

	private ArrayList<Record> records = new ArrayList<Record>();
}
