package com.nonlinearlabs.client;

import java.util.Date;
import java.util.LinkedList;

import com.google.gwt.core.client.JavaScriptObject;
import com.google.gwt.core.client.Scheduler;
import com.google.gwt.core.client.Scheduler.RepeatingCommand;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.HTMLPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.PopupPanel;
import com.nonlinearlabs.client.dataModel.setup.SetupModel;
import com.nonlinearlabs.client.world.Uuid;
import com.nonlinearlabs.client.world.overlay.GWTDialog;

public class WebSocketConnection {
	JavaScriptObject webSocketConnection;
	private Timer pollTimer;
	private RepeatingCommand pingTimeout;
	private int pingCount = 0;
	private Date timeSent;
	private Timer notifyTimer;
	private int delay = 50;
	private LinkedList<String> pendingUpdates = new LinkedList<String>();

	private static class ConnectionErrorPopup extends PopupPanel {

		public ConnectionErrorPopup() {
			super(false);
			Label l = null;
			setWidget(l = new Label("The connection to the Nonlinear Instrument seems to be broken :-("));
			l.getElement().addClassName("dc-text");
			setGlassEnabled(true);
			center();
		}
	}

	private ConnectionErrorPopup connectionErrorPopup = null;

	private void showConnectionError() {
		Tracer.log("showConnectionError");
		if (connectionErrorPopup == null) {
			connectionErrorPopup = new ConnectionErrorPopup();
		}
	}

	private void hideConnectionError() {
		if (connectionErrorPopup != null) {
			connectionErrorPopup.hide();
			connectionErrorPopup = null;
		}
	}

	public interface ServerListener {
		public void onServerConnectionOpened();

		public void onServerUpdate(String text);
	}

	ServerListener listener;
	String clientId;

	public WebSocketConnection() {
		clientId = Uuid.random();
	}

	public void connectToServer(ServerListener listener) {
		Tracer.log("startPolling for clientId " + clientId);
		this.listener = listener;
		if (Window.Location.getPort() == "8888") {
			openWebSocket(Window.Location.getHostName() + ":8080", clientId);
		} else {
			openWebSocket(Window.Location.getHost(), clientId);
		}

		SetupModel.get().systemSettings.deviceName.onChange(t -> {
			if (lastDeviceName != null && !lastDeviceName.isEmpty() && !t.isEmpty() && lastDeviceName != t) {
				notifyHostChanged(lastDeviceName, t);
			}

			lastDeviceName = t;
			return true;
		});
	}

	public native void openWebSocket(String host, String path)
	/*-{
		var address = 'ws://' + host + '/ws/' + path;
		var connection = new WebSocket(address);
		var self = this;
		self.@com.nonlinearlabs.client.WebSocketConnection::webSocketConnection = connection;
	
		connection.onopen = function() {
			self.@com.nonlinearlabs.client.WebSocketConnection::onServerOpen(Ljava/lang/String;Lcom/google/gwt/core/client/JavaScriptObject;)('', connection);
		};
	
		connection.onerror = function(error) {
			self.@com.nonlinearlabs.client.WebSocketConnection::onServerError(Ljava/lang/String;Lcom/google/gwt/core/client/JavaScriptObject;)('error', connection);
		};
	
		connection.onclose = function(e) {
			self.@com.nonlinearlabs.client.WebSocketConnection::onServerClosed(Ljava/lang/String;Lcom/google/gwt/core/client/JavaScriptObject;)('closed', connection);
		};
	
		connection.onmessage = function(e) {
			self.@com.nonlinearlabs.client.WebSocketConnection::onServerMessage(Ljava/lang/String;Lcom/google/gwt/core/client/JavaScriptObject;)(e.data, connection);
		};
	}-*/;

	public void onServerOpen(String data, JavaScriptObject connection) {
		if (connection != webSocketConnection) {
			Tracer.log("Server open notified for outdated connection - ignoring.");
			return;
		}

		pingTimeout = null;
		hideConnectionError();
		ping(connection);
		listener.onServerConnectionOpened();
	}

	public void onServerError(String data, JavaScriptObject connection) {
		if (connection != webSocketConnection) {
			Tracer.log("Server error notified for outdated connection - ignoring.");
			return;
		}

		pingTimeout = null;
		Tracer.log("onServerError");
		reconnect(2000);
		showConnectionError();
	}

	public void onServerClosed(String data, JavaScriptObject connection) {
		if (connection != webSocketConnection) {
			Tracer.log("Server closed notified for outdated connection - ignoring.");
			return;
		}

		pingTimeout = null;
		Tracer.log("onServerClosed");
		reconnect(2000);
		showConnectionError();
	}

	public void onServerMessage(String data, JavaScriptObject connection) {
		if (connection != webSocketConnection) {
			Tracer.log("Server update arrived for outdated connection - ignoring.");
			return;
		}

		if (data.startsWith("/pong/")) {
			onPongReceived(connection);
		} else {
			pendingUpdates.add(data);
			scheduleNotify();
		}
	}

	public void send(String command) {
		doSend(command);
		timeSent = new Date();
	}

	private native void doSend(String command)
	/*-{
		if (this.@com.nonlinearlabs.client.WebSocketConnection::webSocketConnection)
			this.@com.nonlinearlabs.client.WebSocketConnection::webSocketConnection
					.send(command);
	}-*/;

	protected void scheduleNotify() {
		if (notifyTimer != null)
			notifyTimer.cancel();

		if (timeSent != null) {
			Date now = new Date();
			long diff = now.getTime() - timeSent.getTime();

			if (diff >= delay)
				notifyUpdates();
			else
				scheduleServerUpdate((int) (delay - diff));
		} else {
			notifyUpdates();
		}
	}

	private void notifyUpdates() {
		for (String s : pendingUpdates) {
			listener.onServerUpdate(s);
		}

		pendingUpdates.clear();
	}

	private void scheduleServerUpdate(int diff) {
		if (notifyTimer != null)
			notifyTimer.cancel();

		notifyTimer = new Timer() {

			@Override
			public void run() {
				scheduleNotify();
			}
		};

		notifyTimer.schedule(diff);
	}

	protected void ping(JavaScriptObject connection) {
		send("/ping/" + pingCount);

		pingTimeout = new RepeatingCommand() {

			@Override
			public boolean execute() {
				if (pingTimeout == this) {
					Tracer.log("ping timeout");
					showConnectionError();
					reconnect(1000);
				}
				return false;
			}
		};

		Scheduler.get().scheduleFixedPeriod(pingTimeout, 6000);

	}

	private void onPongReceived(JavaScriptObject connection) {
		pingTimeout = null;
		hideConnectionError();
		Scheduler.get().scheduleFixedPeriod(new RepeatingCommand() {

			@Override
			public boolean execute() {
				ping(connection);
				return false;
			}
		}, 2000);
	}

	private void reconnect(int timeout) {
		if (pollTimer == null) {
			pollTimer = new Timer() {
				@Override
				public void run() {
					pollTimer = null;
					connectToServer(listener);
				}
			};

			pollTimer.schedule(timeout);
		}
	}

	static int ssid_left = 0;
	static int ssid_top = 0;
	static SSIDChangedDialog m_dialog = null;

	public class SSIDChangedDialog extends GWTDialog {

		public SSIDChangedDialog() {
			setModal(true);
			setWidth("20em");
			addHeader("C15 SSID change detected!");

			HTMLPanel panel = new HTMLPanel("");
			HTMLPanel buttons = new HTMLPanel("");
			panel.add(new Label(
					"The SSID of the connected C15 instrument has changed. Do you want to reload the Graphical User Interface now?",
					true));

			Button yes, cancelButton;

			buttons.add(cancelButton = new Button("Cancel", new ClickHandler() {

				@Override
				public void onClick(ClickEvent arg0) {
					commit();
				}
			}));

			buttons.add(yes = new Button("Reload", new ClickHandler() {

				@Override
				public void onClick(ClickEvent arg0) {
					Window.Location.reload();
					commit();
				}
			}));

			yes.getElement().addClassName("modal-button-button");
			cancelButton.getElement().addClassName("modal-button-button");
			buttons.getElement().addClassName("modal-button-div");
			panel.add(buttons);
			add(panel);

			m_dialog = this;
		}

		@Override
		protected void setLastPopupPos(int popupLeft, int popupTop) {
			ssid_left = popupLeft;
			ssid_top = popupTop;
		}

		@Override
		protected int getLastPopupPosTop() {
			return ssid_top;
		}

		@Override
		protected int getLastPopupPosLeft() {
			return ssid_left;
		}

		@Override
		protected void commit() {
			hide();
			m_dialog = null;
		}

	};

	private void notifyHostChanged(String oldHost, String newHost) {
		SSIDChangedDialog dialog = new SSIDChangedDialog();
		dialog.center();
	}

	String lastDeviceName = null;

}