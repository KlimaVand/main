package com.af.euroblight.objects;

import org.json.JSONException;
import org.json.JSONObject;

import com.af.euroblight.AppDelegate;

public class CollectionItem {
	
	// JSON keys
	static final String COLLECTION_ITEM_KEY_NAME = "name";
	static final String COLLECTION_ITEM_KEY_VALUE = "value";
	static final String COLLECTION_ITEM_KEY_ID = "id";
	
	// Variables
	private String name;
	private String value;
	private int id;

	public static CollectionItem initializeWithJSONObject(JSONObject json) {
	    if (json == null) {
	        return null;
	    }
	    CollectionItem item = new CollectionItem();
	    try {
			item.setName((String) json.getString(COLLECTION_ITEM_KEY_NAME));
			item.setValue((String)json.getString(COLLECTION_ITEM_KEY_VALUE));
			item.setId((int)json.getInt(COLLECTION_ITEM_KEY_ID));
			return item;
		} catch (JSONException exception) {
			exception.printStackTrace();
			return null;
		}
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getValue() {
		return value;
	}

	public void setValue(String value) {
		this.value = value;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}
	
	/* The toString method is extremely important to making this class work with a Spinner
	 * (or ListView) object because this is the method called when it is trying to represent
	 * this object within the control.  If you do not have a toString() method, you WILL
	 * get an exception.
	 */
	public String toString() {
		return(AppDelegate.getStringResourceByName(getName()));
	}	
}
