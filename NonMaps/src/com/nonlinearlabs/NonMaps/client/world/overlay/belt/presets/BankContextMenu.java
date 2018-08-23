package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.http.client.URL;
import com.google.gwt.user.client.Window;
import com.nonlinearlabs.NonMaps.client.ClipboardManager.ClipboardContent;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.Renameable;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RenameDialog;
import com.nonlinearlabs.NonMaps.client.world.maps.NonPosition;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.Bank;
import com.nonlinearlabs.NonMaps.client.world.overlay.BankInfoDialog;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets.TextUpload.TextUploadedHandler;

import java.util.List;

public abstract class BankContextMenu extends ContextMenu {

	public BankContextMenu(OverlayLayout parent, final Bank bank) {
		super(parent);

		if (hasBankCreationRights()) {
			addChild(new ContextMenuItem(this, "Create New Bank") {
				@Override
				public Control click(Position eventPoint) {
					createNewBank();
					return super.click(eventPoint);
				}
			});

			if (getNonMaps().getNonLinearWorld().getClipboardManager().getClipboardState() != ClipboardContent.empty) {
				addChild(new ContextMenuItem(this, "Paste") {
					@Override
					public Control click(Position eventPoint) {
						NonPosition pos = getNonMaps().getNonLinearWorld().toNonPosition(getParent().getPixRect().getLeftTop());
						getNonMaps().getServerProxy().pasteOnPresetManager(pos);
						return super.click(eventPoint);
					}
				});
			}

			addChild(new ContextMenuItem(this, "Import Bank from File...") {
				@Override
				public Control click(final Position eventPoint) {
					new TextUpload(new TextUploadedHandler() {
						@Override
						public void onTextUploaded(String fileName, String text) {
							NonPosition pos = getNonMaps().getNonLinearWorld().toNonPosition(getParent().getPixRect().getLeftTop());
							NonMaps.theMaps.getServerProxy().importBank(fileName, text, pos);
						}
					}, ".xml");
					return super.click(eventPoint);
				}
			});
		}

		if (bank != null) {

			if (!BankInfoDialog.isShown()) {
				String bankInfoText = "Bank Info ...";
				addChild(new ContextMenuItem(this, bankInfoText) {
					@Override
					public Control click(Position eventPoint) {
						bank.selectBank(true);
						BankInfoDialog.toggle();
						invalidate(INVALIDATION_FLAG_UI_CHANGED);
						return super.click(eventPoint);
					}
				});
			}

			addChild(new ContextMenuItem(this, "Rename ...") {
				@Override
				public Control click(Position eventPoint) {
					RenameDialog.open(bank);
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, "Copy") {
				@Override
				public Control click(Position eventPoint) {
					getNonMaps().getServerProxy().copyBank(bank);
					return super.click(eventPoint);
				}
			});

			if (hasPaste()) {
				if (getNonMaps().getNonLinearWorld().getClipboardManager().getClipboardState() != ClipboardContent.empty) {
					addChild(new ContextMenuItem(this, "Paste") {
						@Override
						public Control click(Position eventPoint) {
							getNonMaps().getServerProxy().pasteOnBank(bank);
							return super.click(eventPoint);
						}
					});
				}
			}

			if (bank.getParent().canSelectBankWithOrdernumberOffset(bank, -1)) {
				addChild(new ContextMenuItem(this, "Move Left") {
					@Override
					public Control click(Position eventPoint) {
						getNonMaps().getServerProxy().moveBy(bank, "LeftByOne");
						return super.click(eventPoint);
					}
				});
			}

			if (bank.getParent().canSelectBankWithOrdernumberOffset(bank, 1)) {
				addChild(new ContextMenuItem(this, "Move Right") {
					@Override
					public Control click(Position eventPoint) {
						getNonMaps().getServerProxy().moveBy(bank, "RightByOne");
						return super.click(eventPoint);
					}
				});
			}

			addChild(new ContextMenuItem(this, "Delete") {
				@Override
				public Control click(Position eventPoint) {
					getNonMaps().getServerProxy().deleteBank(bank);
					return super.click(eventPoint);
				}
			});

			if (hasMinimize()) {
				addChild(new ContextMenuItem(this, bank.isMinimized() ? "Full Size" : "Minimize") {
					@Override
					public Control click(Position eventPoint) {
						bank.toggleMinMax();
						return super.click(eventPoint);
					}
				});
			}

			addChild(new ContextMenuItem(this, "Export as File ...") {
				@Override
				public Control click(Position eventPoint) {
					String bankName = URL.encodePathSegment(bank.getCurrentName());
					String uri = "/presets/banks/download-bank/" + bankName + ".xml?uuid=" + bank.getUUID();
					Window.open(uri, "", "");
					return super.click(eventPoint);
				}
			});

			addChild(new ContextMenuItem(this, "Sort Bank Numbers") {
				@Override
				public Control click(Position eventPoint) {
					getNonMaps().getServerProxy().sortBankNumbers();
					return super.click(eventPoint);
				}
			});
		}
	}

	private NonPosition calcualteBankPosition() {
		List<Bank> banks = NonMaps.get().getNonLinearWorld().getPresetManager().getBanks();

		NonPosition np = new NonPosition();
		if(!banks.isEmpty()) {
			Bank rightMost = banks.get(0);
			for(Bank b : banks)
			{
				double currentX = b.getNonPosition().getLeft();
				double rightMostX = rightMost.getNonPosition().getLeft();
				if(currentX > rightMostX)
					rightMost = b;
			}
			return new NonPosition(rightMost.getNonPosition().getLeft() + 300, rightMost.getNonPosition().getTop());
		}
		return new NonPosition(0,0);
	}

	private void createNewBank() {
		NonPosition bankPosition = calcualteBankPosition();
		PresetManagerContextMenu.createNewBank(bankPosition);
	}

	protected abstract boolean hasMinimize();

	protected abstract boolean hasPaste();

	protected abstract boolean hasBankCreationRights();
}
