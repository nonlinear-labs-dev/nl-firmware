package com.nonlinearlabs.client.world.overlay.undo;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.client.Millimeter;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.world.Dimension;
import com.nonlinearlabs.client.world.Position;
import com.nonlinearlabs.client.world.Rect;

public class UndoTransaction {

	private long id;
	private long defaultRedoRoute = 0;
	private UndoTransaction predecessor;
	private ArrayList<UndoTransaction> successors = new ArrayList<UndoTransaction>();
	private String title;

	private boolean isOnDragRoute = false;
	private boolean isOnCurrentRoute = false;
	private double titleWidth = 0;
	private Layer layer = null;

	public static HashMap<Long, UndoTransaction> allTransactions = new HashMap<Long, UndoTransaction>();

	public UndoTransaction(UndoTree undoTree, UndoTransaction predecessor, final long id) {
		this.id = id;
		this.predecessor = predecessor;
		allTransactions.put(id, this);

		int depth = (predecessor != null) ? predecessor.getDepth() + 1 : 0;
		layer = undoTree.getLayers().getLayer(depth);
		layer.add(this);
	}

	public int getDepth() {
		return layer.getDepth();
	}

	public static double getEntryHeight() {
		return Millimeter.toPixels(10);
	}

	public static double getSpaceBetweenEntries() {
		return Millimeter.toPixels(10);
	}

	public long getID() {
		return id;
	}

	public String getTitle() {
		return title;
	}

	public long getDefaultRedoRoute() {
		return defaultRedoRoute;
	}

	public UndoTransaction findTransaction(long id) {
		return allTransactions.get(id);
	}

	public void update(long id, Node node) {
		if (this.id != id) {
			allTransactions.remove(this.id);
			this.id = id;
			resetPredecessor();
			allTransactions.put(this.id, this);
		}

		handleDeadTransactions(node);

		String redoRoute = node.getAttributes().getNamedItem("default-redo-route").getNodeValue();
		defaultRedoRoute = Long.decode(redoRoute);
		String title = node.getAttributes().getNamedItem("name").getNodeValue();

		if (!title.equals(this.title)) {
			titleWidth = 0;
			this.title = title;
			layer.layers.updateMaxWidth(layer);
		}
	}

	public void resetPredecessor() {
		this.predecessor = null;
	}

	private void handleDeadTransactions(Node node) {
		HashMap<Long, UndoTransaction> doomed = getCurrentSuccessors();
		surviveLivingSuccessors(doomed, node);
		removeDoomedSuccessors(doomed);
	}

	private HashMap<Long, UndoTransaction> getCurrentSuccessors() {
		HashMap<Long, UndoTransaction> doomed = new HashMap<Long, UndoTransaction>();

		for (UndoTransaction c : successors) {
			doomed.put(c.id, c);
		}
		return doomed;
	}

	private void surviveLivingSuccessors(HashMap<Long, UndoTransaction> doomed, Node node) {
		String successors = node.getAttributes().getNamedItem("successors").getNodeValue();
		String splits[] = successors.split(",");

		for (String split : splits)
			if (split != null && !split.isEmpty())
				doomed.remove(Long.decode(split));
	}

	private void removeDoomedSuccessors(HashMap<Long, UndoTransaction> doomed) {
		for (Map.Entry<Long, UndoTransaction> e : doomed.entrySet()) {
			UndoTransaction u = e.getValue();
			successors.remove(u);
			u.delete(null);
		}
	}

	public void delete(UndoTransaction except) {
		for (UndoTransaction s : successors)
			if (s != except)
				s.delete(except);

		layer.remove(this);
		allTransactions.remove(id);
	}

	boolean isCurrentUndoTransaction() {
		UndoTree tree = getUndoTree();
		long currentUndo = tree.getCurrentUndoID();
		UndoTransaction root = tree.getRoot();
		if (currentUndo == 0 && root == this)
			return true;

		return currentUndo == id;
	}

	public void erase() {
		NonMaps.theMaps.getServerProxy().eraseUndoBranch(id);
	}

	public boolean isParentOfCurrentUndo() {
		UndoTree tree = getUndoTree();
		UndoTransaction walker = tree.getCurrentUndo();

		while (walker != null) {
			if (walker == this)
				return true;

			if (walker.getPredecessor() == null)
				break;

			walker = walker.getPredecessor();
		}

		return false;
	}

	public UndoTransaction getPredecessor() {
		return predecessor;
	}

	public ArrayList<UndoTransaction> getSuccessors() {
		return successors;
	}

	private UndoTree getUndoTree() {
		return NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getUndoTree().getTree();
	}

	public UndoTransaction getTransaction(long id) {
		return findTransaction(id);
	}

	public long getUndoTransactionID() {
		return id;
	}

	public void rebase() {
		NonMaps.theMaps.getServerProxy().rebaseUndo(id);
	}

	public double getTitleTextWidth(Context2d ctx) {
		if (titleWidth == 0)
			titleWidth = ctx.measureText(title).getWidth();

		return titleWidth;
	}

	public void addSuccessor(UndoTree undoTree, long id, Node node) {
		UndoTransaction a = new UndoTransaction(undoTree, this, id);
		getSuccessors().add(a);
		a.update(id, node);
	}

	public void setupDragRoute() {
		for (UndoTransaction a : allTransactions.values())
			a.unsetDragRoute();

		setupDragRouteUpstream();
		setupDragRouteDownstream();
	}

	private void unsetDragRoute() {
		isOnDragRoute = false;
	}

	private void setupDragRouteDownstream() {
		UndoTransaction route = this;

		while (route != null) {
			route.isOnDragRoute = true;
			route = findTransaction(route.defaultRedoRoute);
		}
	}

	private void setupDragRouteUpstream() {
		UndoTransaction route = this;

		while (route != null) {
			route.isOnDragRoute = true;
			route = route.getPredecessor();
		}
	}

	public long getDragRoute() {
		for (UndoTransaction u : successors)
			if (u.isOnDragRoute)
				return u.id;

		return 0;
	}

	public void setupCurrentRoute() {
		for (UndoTransaction a : allTransactions.values())
			a.unsetCurrentRoute();

		UndoTransaction route = this;

		while (route != null) {
			route.setOnCurrentRoute(true);
			route = route.getPredecessor();
		}

		route = this;

		while (route != null) {
			route.setOnCurrentRoute(true);
			route = findTransaction(route.defaultRedoRoute);
		}
	}

	private void unsetCurrentRoute() {
		setOnCurrentRoute(false);
	}

	public boolean isOnCurrentRoute() {
		return isOnCurrentRoute;
	}

	private void setOnCurrentRoute(boolean isOnCurrentRoute) {
		this.isOnCurrentRoute = isOnCurrentRoute;
	}

	public void jump() {
		NonMaps.get().getServerProxy().undoJump(id);
	}

	public int countLayers(int maxNumTreeLayers) {
		int maxFoundLayers = 1;

		if (maxNumTreeLayers > 0) {
			for (UndoTransaction u : successors) {
				int layersInSubtree = u.countLayers(maxNumTreeLayers - 1) + 1;
				if (layersInSubtree > maxFoundLayers)
					maxFoundLayers = layersInSubtree;
			}
		}
		return maxFoundLayers;
	}

	public Rect calcPixRect(Dimension scrollOffset, Position position) {
		double y = getDepth() * getEntryHeight() + scrollOffset.getHeight() + position.getY();
		double x = layer.getX(this) + scrollOffset.getWidth() + position.getX();
		double w = getTitleTextWidth(layer.layers.getContext());
		double h = getEntryHeight();
		return new Rect(x, y, w, h);
	}

	public Layer getLayer() {
		return layer;
	}
}
