package com.af.euroblight2;

import com.af.euroblight2.managers.CollectionsManager;

import android.app.Application;
import android.content.Context;

public class AppDelegate extends Application {
	
	public enum SUBVIEWS_DEFINITION { REPORT, 
									  MAP, 
									  PRESSURE,
									  ERROR };
	// Variables
	private static Context context;
	private static AppDelegate delegate;
	private SUBVIEWS_DEFINITION[] subviews =  
			new SUBVIEWS_DEFINITION[] { SUBVIEWS_DEFINITION.REPORT, 
										SUBVIEWS_DEFINITION.MAP, 
										SUBVIEWS_DEFINITION.PRESSURE,
										SUBVIEWS_DEFINITION.ERROR };
	
	/** Override Methods */
	
	@Override
	public void onCreate(){
        super.onCreate();
        AppDelegate.delegate = this;
        AppDelegate.context = getApplicationContext();
        // Initialize collectionsManager
        CollectionsManager.sharedManager();
    }
	
	/** Public Methods */
    
	public static Context getContext() {
        return AppDelegate.context;
    }
    
    public static AppDelegate getDelegate() {
        return AppDelegate.delegate;
    }
    
    public SUBVIEWS_DEFINITION[] getSubviews() {
        return subviews;
    }
    
    public static String getStringResourceByName(String name) {
        String packageName = delegate.getPackageName();
        int resId = delegate.getResources().getIdentifier(name, "string", packageName);
        return (resId == 0) ? name : delegate.getString(resId);
    }
}