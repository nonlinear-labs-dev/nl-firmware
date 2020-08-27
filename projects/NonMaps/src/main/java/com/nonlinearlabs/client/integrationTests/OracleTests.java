package com.nonlinearlabs.client.integrationTests;

import com.google.gwt.user.client.DOM;
import com.nonlinearlabs.client.NonMaps;
import com.nonlinearlabs.client.ServerProxy;
import com.nonlinearlabs.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.client.world.maps.NonPosition;
import com.nonlinearlabs.client.world.maps.presets.PresetManager;
import com.nonlinearlabs.client.WebSocketConnection;

public class OracleTests extends TestWithSteps {

    String bankName = "Oracle-testbank-" + DOM.createUniqueId();
    WebSocketConnection webSocket;
    ServerProxy server = NonMaps.get().getServerProxy();
    PresetManager pm = NonMaps.get().getNonLinearWorld().getPresetManager();
    
    OracleTests() {

        createWebSocket();

        createBank();

        createPresets();

        selectPresetViaProxy();
        selectPresetViaNewWebSocket();

        deleteBank();

        start();
    }

    private void selectPresetViaNewWebSocket() {
        addStep(() -> {
            webSocket.send("/presets/banks/select-preset?uuid=" + findBank().getPreset(0).getUUID().toString() + "&isOracle=true");
        }, () -> {
            boolean wasOracleOfServer = server.lastDocumentCouldOmitOracles();
            return !wasOracleOfServer;
        });
    }

    private void selectPresetViaProxy() {
        addStep(() -> {
            server.selectPreset(findBank().getPreset(1).getUUID());
        }, () -> {
            boolean wasOracleOfServer = server.lastDocumentCouldOmitOracles();
            return wasOracleOfServer;
        });
    }

    private void createWebSocket() {
        webSocket = new WebSocketConnection();
		webSocket.connectToServer(new WebSocketConnection.ServerListener() {

			@Override
			public void onServerUpdate(String text) {
			}

			@Override
			public void onServerConnectionOpened() {
			}
		});        
    }

    private void createBank() {
        addStep(() -> {
            server.newBank(bankName, new NonPosition());
        }, () -> {
            return findBank() != null;
        });
    }

    private void createPresets() {
        addStep(() -> {
            Bank b = findBank();
            server.appendEditBuffer(b);
            server.appendEditBuffer(b);
            server.appendEditBuffer(b);
        }, () -> {
            return findBank().getPresetList().getPresetCount() == 3;
        });
    }

    private Bank findBank() {
        PresetManager pm = NonMaps.get().getNonLinearWorld().getPresetManager();
        for (Bank b : pm.getBanks())
            if (b.getCurrentName() == bankName)
                return b;

        return null;
    }

    private void deleteBank() {
        addStep(() -> {
            Bank b = findBank();
            server.deleteBank(b);
        }, () -> {
            return findBank() == null;
        });
    }
}
