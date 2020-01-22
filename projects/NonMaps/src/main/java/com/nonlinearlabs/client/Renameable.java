package com.nonlinearlabs.client;

public interface Renameable {

	public abstract String getCurrentName();

	public abstract String getTitleName();

	public abstract String getEntityName();

	public abstract void setName(String newName);

}