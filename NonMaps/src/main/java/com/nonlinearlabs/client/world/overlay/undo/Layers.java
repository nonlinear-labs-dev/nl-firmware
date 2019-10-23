package com.nonlinearlabs.client.world.overlay.undo;

import java.util.ArrayList;
import java.util.function.Predicate;

import com.google.gwt.canvas.client.Canvas;
import com.google.gwt.canvas.dom.client.Context2d;

public class Layers {

	private Context2d widthMeasureContext;
	ArrayList<Layer> layers = new ArrayList<Layer>();
	double maxLayerWidth = 0;

	public Layers() {
		widthMeasureContext = Canvas.createIfSupported().getContext2d();
	}

	private double calcMaxLayerWidth() {
		double max = 0;

		for (Layer layer : layers)
			max = Math.max(max, layer.getLayerWidth(widthMeasureContext));

		return max;
	}

	public Context2d getContext() {
		return widthMeasureContext;
	}

	public Layer getLayer(int depth) {
		if (depth < 0)
			return null;

		if (layers.size() <= depth) {
			Layer l = new Layer(this, depth);
			layers.add(l);
			return l;
		}

		return layers.get(depth);
	}

	public void updateMaxWidth(Layer layer) {
		maxLayerWidth = Math.max(maxLayerWidth, layer.getLayerWidth(widthMeasureContext));
	}

	public void updateMaxWidth() {
		maxLayerWidth = calcMaxLayerWidth();
	}

	public double getMaxLayerWidth() {
		return maxLayerWidth;
	}

	public int getNumLayers() {
		return layers.size();
	}

	public void onRootChanged(UndoTransaction root) {
		layers.removeIf(new Predicate<Layer>() {

			@Override
			public boolean test(Layer layer) {
				return layer.getSize() == 0;
			}
		});

		int depth = 0;
		for (Layer l : layers)
			l.depth = depth++;
	}
}
