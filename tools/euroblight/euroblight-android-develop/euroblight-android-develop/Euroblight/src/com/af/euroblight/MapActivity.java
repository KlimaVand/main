package com.af.euroblight;

import java.lang.ref.WeakReference;

import com.af.euroblight.handlers.CommunicationManagerHandler;
import com.af.euroblight.managers.CommunicationManager;
import com.af.euroblight.managers.DataManager;
import com.af.euroblight.objects.CustomSpinner;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.ProgressDialog;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.TextView;
import android.support.v4.app.NavUtils;

@SuppressLint("SetJavaScriptEnabled")
public class MapActivity extends Activity implements OnItemSelectedListener, CommunicationManagerHandler {
	
	static final String MAP_HTML_RESOURCE_LOCATION = "file:///android_asset/map.html";
	static final String MAP_JAVASCRIPT_FUNCTION_BIND = "javascript:drawMap('%s')";
	
	// GUI Variables
	LinearLayout linearLayoutLocations;
	ScrollView scrollViewMapInfo;
	CustomSpinner spinnerLocations;
	TextView textViewErrorMessage;
	WebView webView;
	ProgressDialog progressDialog;
	
	// Variables 
	private final Handler uiThreadHandler = new Handler(Looper.getMainLooper());
	private String currentLocation; 
	private String[] locationKeys = new String[] { "DK", 
												   "FI",
												   "NO",
												   "SE" };
			
	/** Override Methods */
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_map);
		linearLayoutLocations = (LinearLayout)findViewById(R.id.linearLayoutMapLocations);
		scrollViewMapInfo = (ScrollView)findViewById(R.id.scrollViewMapInfo);
		spinnerLocations = (CustomSpinner)findViewById(R.id.spinnerLocations);
		
		// Code to prevent the spinner from be fired twice 
		final WeakReference<MapActivity> weakThis = new WeakReference<MapActivity>(this);
		spinnerLocations.post(new Runnable() {
		    public void run() {
		    	spinnerLocations.setOnItemSelectedListener(weakThis.get());
		    }
		});
		
		textViewErrorMessage = (TextView)findViewById(R.id.textViewErrorMessage);
		webView = (WebView)findViewById(R.id.webViewMap);
		// Show the Up button in the action bar.
		setupActionBar();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
	}
		
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.map, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case android.R.id.home:
			// This ID represents the Home or Up button. In the case of this
			// activity, the Up button is shown. Use NavUtils to allow users
			// to navigate up one level in the application structure. For
			// more details, see the Navigation pattern on Android Design:
			//
			// http://developer.android.com/design/patterns/navigation.html#up-vs-back
			//
			NavUtils.navigateUpFromSameTask(this);
			return true;
		case R.id.menu_action_refresh:
			retrieveKmlFile(currentLocation);
			return true;
		case R.id.menu_action_info: 
			showInfo();
			return true;
		case R.id.menu_action_locations:
			showLocations();
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	/** Private Methods */
	
	/**
	 * Set up the {@link android.app.ActionBar}.
	 */
	private void setupActionBar() {
		getActionBar().setDisplayHomeAsUpEnabled(true);
	}
	
	private void showInfo() {
		int visibilityStatus = scrollViewMapInfo.getVisibility();
		scrollViewMapInfo.setVisibility(
			(visibilityStatus == View.VISIBLE) ?  
					View.INVISIBLE : View.VISIBLE
					);
	}
	
	private void showLocations() {
		int visibilityStatus = linearLayoutLocations.getVisibility();
		linearLayoutLocations.setVisibility(
			(visibilityStatus == View.VISIBLE) ?  
					View.INVISIBLE : View.VISIBLE
					);
	}
	
	private void showErrorMessage(String errorMessage) {
		webView.clearView();
		if (errorMessage == null || errorMessage.isEmpty()) {
			textViewErrorMessage.setText("");
			textViewErrorMessage.setVisibility(View.INVISIBLE);
			return;
		}
		textViewErrorMessage.setText(errorMessage);
		textViewErrorMessage.setVisibility(View.VISIBLE);
	}
	
	private void showProgressDialog(final String message) {
		hideProgressDialog();
		if (progressDialog == null) {
			progressDialog = new ProgressDialog(this);
			progressDialog.setCancelable(false);
		}
		uiThreadHandler.post(new Runnable() {
			@Override
			public void run() {
				progressDialog.setMessage(message);
				progressDialog.show();
			}
		});
	}
	
	private void hideProgressDialog() {
		uiThreadHandler.post(new Runnable() {
			@Override
			public void run() {
				if (progressDialog.isShowing()) {
					progressDialog.cancel();	
				}
			}
		});
	}
	
	private void retrieveKmlFile(String locationKey) {
		if (!CommunicationManager.isCommunicationPossible()) { 
			this.showErrorMessage(getString(R.string.error_text_no_connection_available));
			return;
		} 
		currentLocation = locationKey;
		showProgressDialog(getString(R.string.progress_dialod_please_wait_retrieve_data));
		CommunicationManager communicationManager = 
				new CommunicationManager();
		String username = DataManager.sharedManager().storedUsername();
		String password = DataManager.sharedManager().storedPassword();
		communicationManager.retrieveKMLFile(username, password, locationKey, this);
	}

	private void loadKmlFile(final String kmlFileUrl) {
		webView.getSettings().setJavaScriptEnabled(true);
		webView.clearView();
	    webView.loadUrl(MAP_HTML_RESOURCE_LOCATION);
	    webView.setWebViewClient(new WebViewClient() {
	        public void onPageFinished(WebView view, String loc) {
	        	webView.loadUrl(String.format(MAP_JAVASCRIPT_FUNCTION_BIND,kmlFileUrl));
	        }
	    });
	}
			
	/** OnItemSelectedListener Methods */
	
	@Override
	public void onItemSelected(AdapterView<?> adapter, View view, int position, long i) {
		retrieveKmlFile(locationKeys[position]);
	}

	@Override
	public void onNothingSelected(AdapterView<?> arg0) {
		// Fall through.
	}

	/** CommunicationManagerHandler Methods */
	
	@Override
	public void successHandler(final String result) {	
		uiThreadHandler.post(new Runnable() {
	        @Override
	        public void run() {
	        	showErrorMessage("");
	        	loadKmlFile(result);
	        	hideProgressDialog();
	        }
	    });
	}

	@Override
	public void errorHandler(final ERRORS_DEFINITION_CODE errorCode) {
		uiThreadHandler.post(new Runnable() {
	        @Override
	        public void run() {
	        	int stringErrorId;
	        	switch (errorCode) {
	        	case ERROR_UNAUTHORIZED:
	        		stringErrorId = R.string.error_text_unauthorized;
	        		break;
	        	case PARSE_ERROR:
	        		stringErrorId = R.string.map_text_no_records_to_display;
	        		break;
	        	default:
	        		stringErrorId = R.string.map_text_no_records_to_display;
	        		break;
	        	}
	        	String errorMessage = getString(stringErrorId);
	        	showErrorMessage(errorMessage);
	        	hideProgressDialog();
	        }
	    });
	}
}