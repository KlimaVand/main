package com.af.euroblight2.managers;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.net.ParseException;

import com.af.euroblight2.AppDelegate;
import com.af.euroblight2.objects.CollectionItem;

public class CollectionsManager {

	public static enum COLLECTION_ID { DENMARK_POSTAL_CODE,
									    TYPE_OF_PRODUCTION,
										 		      BBCH,
										 		  SEVERITY };
	
	// Variables
	private static CollectionsManager sharedManager; 
	private static ArrayList<CollectionItem> denmarkLocationsList;
	private static ArrayList<CollectionItem> typeOfProductionList;
	private static ArrayList<CollectionItem> bbchList;
	private static ArrayList<CollectionItem> severityList;
		
	// Keys
	private final String DENMARK_POSTAL_CODE_PATH = "infection_pressure_locations_dk.json";
	private final String TYPE_OF_PRODUCTION_PATH = "type_of_production.json";
	private final String BBCH_PATH = "type_of_bbch.json";
	private final String SEVERITY_PATH = "type_of_severity.json";
			
	/** Public Methods */
	
	public static  CollectionsManager sharedManager() {
		if (sharedManager == null) {
			sharedManager = new  CollectionsManager();
			Thread readingThread = new Thread(new Runnable(){
				@Override
				public void run() {
					denmarkLocationsList = sharedManager.getCollection(COLLECTION_ID.DENMARK_POSTAL_CODE);
					typeOfProductionList = sharedManager.getCollection(COLLECTION_ID.TYPE_OF_PRODUCTION);
					bbchList = sharedManager.getCollection(COLLECTION_ID.BBCH);
					severityList = sharedManager.getCollection(COLLECTION_ID.SEVERITY);
				}
			});
			readingThread.run();
		}
		return sharedManager;
	}	
	
	public ArrayList<CollectionItem> getDenmarkLocations() {
		return denmarkLocationsList;
	}
	
	public ArrayList<CollectionItem> getTypeOfProductionList() {
		return typeOfProductionList;
	}
	
	public ArrayList<CollectionItem> getBBCHList() {
		return bbchList;
	}
	
	public ArrayList<CollectionItem> getSeverityList() {
		return severityList;
	}
	
	public ArrayList<CollectionItem> getCollection(COLLECTION_ID id) {
		String jsonString = loadJSONFromAsset(getCollectionPath(id));
		ArrayList<CollectionItem> collection = new ArrayList<CollectionItem>();
		try {
			JSONArray jsonArray = new JSONArray(jsonString);
			 for (int i = 0; i < jsonArray.length(); i++) {
				 JSONObject jsonObject = jsonArray.getJSONObject(i);
				 collection.add(CollectionItem.initializeWithJSONObject(jsonObject));
		     }
		} catch (ParseException e) {
			e.printStackTrace();
		} catch (JSONException e) {
			e.printStackTrace();
		}
		return collection;	
	}
	
	/** Private Methods */
	
	private String loadJSONFromAsset(String assetPath) {
	    String json = null;
	    try {
	    	InputStream inputStream = AppDelegate.getContext().getAssets().open(assetPath);
	        int size = inputStream.available();
	        byte[] buffer = new byte[size];
	        inputStream.read(buffer);
	        inputStream.close();
	        json = new String(buffer, "UTF-8");
	    } catch (IOException ex) {
	        ex.printStackTrace();
	        return null;
	    }
	    return json;
	}
	
	private String getCollectionPath(COLLECTION_ID id) {
		switch (id) {
		case DENMARK_POSTAL_CODE:
			return DENMARK_POSTAL_CODE_PATH;
		case TYPE_OF_PRODUCTION:
			return TYPE_OF_PRODUCTION_PATH;
		case BBCH:
			return BBCH_PATH;
		case SEVERITY:
			return SEVERITY_PATH;
		default:
			return "";
		}
	}
}