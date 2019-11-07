package com.nonlinearlabs.client.world.overlay.undo;

import java.util.ArrayList;

import com.google.gwt.canvas.dom.client.Context2d;

public class Layer {
	Layers layers;
	ArrayList<UndoTransaction> elements = new ArrayList<UndoTransaction>();
	int depth;

	public Layer(Layers layers, int depth) {
		this.layers = layers;
		this.depth = depth;
	}

	public int getSize() {
		return elements.size();
	}

	public void add(UndoTransaction undoTransaction) {
		Layer parentLayer = layers.getLayer(depth - 1);
		if (parentLayer != null) {
			UndoTransaction predecessor = undoTransaction.getPredecessor();

			if (predecessor != null) {
				UndoTransaction leftNeighbour = parentLayer.getLeftNeighbourWithChildren(predecessor);
				UndoTransaction rightNeighbour = parentLayer.getRightNeighbourWithChildren(predecessor);

				if (leftNeighbour != null) {
					int lastChildOfParentsNeighbourIndex = -1;

					for (int i = 0; i < elements.size(); i++) {
						UndoTransaction a = elements.get(i);
						if (a.getPredecessor() == leftNeighbour) {
							lastChildOfParentsNeighbourIndex = i;
						}
					}

					if (lastChildOfParentsNeighbourIndex >= 0) {
						elements.add(lastChildOfParentsNeighbourIndex + 1, undoTransaction);
						return;
					}
				}

				if (rightNeighbour != null) {
					int firstChildOfParentsNeighbourIndex = -1;

					for (int i = 0; i < elements.size(); i++) {
						UndoTransaction a = elements.get(i);
						if (a.getPredecessor() == rightNeighbour) {
							firstChildOfParentsNeighbourIndex = i;
							break;
						}
					}

					if (firstChildOfParentsNeighbourIndex >= 0) {
						elements.add(firstChildOfParentsNeighbourIndex, undoTransaction);
						return;
					}
				}
			}
		}
		elements.add(undoTransaction);
	}

	private UndoTransaction getRightNeighbourWithChildren(UndoTransaction p) {
		boolean foundSelf = false;
		for (int i = 0; i < elements.size(); i++) {
			UndoTransaction a = elements.get(i);

			if (a == p)
				foundSelf = true;
			else if (foundSelf)
				if (!a.getSuccessors().isEmpty())
					return a;

		}
		return null;
	}

	private UndoTransaction getLeftNeighbourWithChildren(UndoTransaction p) {
		UndoTransaction result = null;
		for (int i = 0; i < elements.size(); i++) {
			UndoTransaction a = elements.get(i);

			if (a == p)
				break;

			if (!a.getSuccessors().isEmpty())
				result = a;

		}
		return result;
	}

	public void remove(UndoTransaction undoTransaction) {
		double previousWidth = getLayerWidth(layers.getContext());
		elements.remove(undoTransaction);

		if (previousWidth == layers.getMaxLayerWidth()) {
			layers.updateMaxWidth();
		}
	}

	public double getLayerWidth(Context2d ctx) {
		double w = 2 * UndoTransaction.getSpaceBetweenEntries();

		for (UndoTransaction u : elements) {
			w += u.getTitleTextWidth(ctx);
			w += UndoTransaction.getSpaceBetweenEntries();
		}

		if (!elements.isEmpty())
			w -= UndoTransaction.getSpaceBetweenEntries();

		return w;
	}

	public int getDepth() {
		return depth;
	}

	public double getX(UndoTransaction undoTransaction) {
		double x = UndoTransaction.getSpaceBetweenEntries()
				+ (layers.getMaxLayerWidth() - getLayerWidth(layers.getContext())) / 2;

		for (UndoTransaction u : elements) {
			if (u == undoTransaction)
				return x;

			x += u.getTitleTextWidth(layers.getContext());
			x += UndoTransaction.getSpaceBetweenEntries();
		}

		return x;
	}

}
