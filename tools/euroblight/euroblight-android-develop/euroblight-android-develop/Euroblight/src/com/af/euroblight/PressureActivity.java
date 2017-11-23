package com.af.euroblight;

import java.io.IOException;
import java.lang.ref.WeakReference;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.ArrayList;

import com.af.euroblight.handlers.CommunicationManagerHandler;
import com.af.euroblight.managers.CollectionsManager;
import com.af.euroblight.managers.CommunicationManager;
import com.af.euroblight.managers.DataManager;
import com.af.euroblight.objects.CollectionItem;
import com.af.euroblight.objects.CustomSpinner;
import com.af.euroblight.objects.TouchImageView;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.app.Activity;
import android.app.ProgressDialog;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.view.Display;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.support.v4.app.NavUtils;

public class PressureActivity extends Activity implements OnItemSelectedListener, CommunicationManagerHandler {
	
	// GUI Variables
	LinearLayout linearLayoutLocations;
	CustomSpinner spinnerLocations;
	TextView textViewErrorMessage;
	AutoCompleteTextView autoComplete;
	TouchImageView imageViewPressure;
	ProgressDialog progressDialog;
		
	// Variables 
	private final Handler uiThreadHandler = new Handler(Looper.getMainLooper());
	private CollectionItem currentLocation; 
	private Display display;
	private Bitmap bmp;
	
	ArrayAdapter<?> spinnerLocationsAdapter;
	ArrayAdapter<?> autoCompleteLocationsAdapter;
	private final static Object TAG_LOCATIONS = new Object();
	
	/** Override Methods */
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_pressure);
		linearLayoutLocations = (LinearLayout)findViewById(R.id.linearLayoutPressureLocations);
		spinnerLocations = (CustomSpinner)findViewById(R.id.spinnerLocations);
		autoComplete = (AutoCompleteTextView)findViewById(R.id.autoCompleteTextViewLocation);
		setupLocationsSpinner();
		textViewErrorMessage = (TextView)findViewById(R.id.textViewErrorMessage);
		//webView = (WebView)findViewById(R.id.webViewPressure);
		imageViewPressure = (TouchImageView)findViewById(R.id.imageViewPressure);
		display = getWindowManager().getDefaultDisplay();
		
		// Show the Up button in the action bar.
		setupActionBar();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
	    super.onConfigurationChanged(newConfig);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.pressure, menu);
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
			if (currentLocation != null) {  
				retrieveInfectionPressureChart(currentLocation);
			}
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
	
	private void setupLocationsSpinner() {
		CollectionsManager collectionsManager = CollectionsManager.sharedManager();
		ArrayList<CollectionItem> collection = collectionsManager.getDenmarkLocations();
		spinnerLocationsAdapter = setupArrayAdapterForCollection(collection);
		autoCompleteLocationsAdapter =  setupArrayAdapterForCollection(collection);
		spinnerLocations.setAdapter(spinnerLocationsAdapter);
		spinnerLocations.setTag(TAG_LOCATIONS);
		autoComplete.setAdapter(autoCompleteLocationsAdapter);
		autoComplete.setOnItemClickListener(new AdapterView.OnItemClickListener() {
		      @Override
		      public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
		    	  CollectionItem item = (CollectionItem)parent.getItemAtPosition(position);
		          loadLocation(item.getName());
		          autoComplete.setText("");
		      }
		});
		// Code to prevent the spinner from be fired twice 
		final WeakReference<PressureActivity> weakThis = new WeakReference<PressureActivity>(this);
		spinnerLocations.post(new Runnable() {
			public void run() {
				spinnerLocations.setOnItemSelectedListener(weakThis.get());
			}
		});
	}
	
	private void loadLocation(String locationName) {
		for(int i=0 ; i < spinnerLocationsAdapter.getCount() ; i++) {
			CollectionItem item = (CollectionItem)spinnerLocationsAdapter.getItem(i);
			if (item.getName().equalsIgnoreCase(locationName)) {
				spinnerLocations.setSelection(i);
				return;
			}
		}
	}
	
	private ArrayAdapter<CollectionItem> setupArrayAdapterForCollection(
			final ArrayList<CollectionItem> collection) {
		return new ArrayAdapter<CollectionItem>(this,  
												android.R.layout.simple_spinner_dropdown_item, 
											    collection) {
			@Override
		    public View getDropDownView(final int position, View convertView, ViewGroup parent) {
		        if (convertView == null) {
		        	convertView = View.inflate(getContext(),
		        				 R.layout.multiline_spinner_dropdown_item, 
                            		               null);
		        }

		        TextView item = (TextView) convertView.findViewById(R.id.textview_spinner);
		        String text = collection.get(position).toString();
		        item.setText(text);
		        final TextView finalItem = item;
		        item.post(new Runnable() {
		            @Override
		            public void run() {
		                finalItem.setSingleLine(false);
		            }
		        });
		        return convertView;
		    }
		};
	}
	
	private void showLocations() {
		int visibilityStatus = linearLayoutLocations.getVisibility();
		linearLayoutLocations.setVisibility(
			(visibilityStatus == View.VISIBLE) ?  
					View.INVISIBLE : View.VISIBLE
					);
	}
	
	private void showErrorMessage(String errorMessage) {
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
	
	private void retrieveInfectionPressureChart(CollectionItem location) {
		if (!CommunicationManager.isCommunicationPossible()) { 
			this.showErrorMessage(getString(R.string.error_text_no_connection_available));
			return;
		} 
		currentLocation = location;
		showProgressDialog(getString(R.string.progress_dialod_please_wait_retrieve_data));
		CommunicationManager communicationManager = 
				new CommunicationManager();
		String username = DataManager.sharedManager().storedUsername();
		String password = DataManager.sharedManager().storedPassword();
		Point size = new Point();
		display.getSize(size);
		communicationManager.retrieveInfectionPressure( username, 
														password, 
														location.getName(), 
														location.getValue(),
														(int)(675), 
														(int)(235), 
														this );
	}

	private void loadInfectionPressureChart(final String infectionPressureChartUrl) {
		imageViewPressure.setImageBitmap(bmp);
		imageViewPressure.setZoom(1);
	}
	
	/** OnItemSelectedListener Methods */

	@Override
	public void onItemSelected(AdapterView<?> adapter, View view, int position, long i) {
		retrieveInfectionPressureChart((CollectionItem)adapter.getItemAtPosition(position));
	}

	@Override
	public void onNothingSelected(AdapterView<?> arg0) {
		// Fall through.
	}
	
	/** CommunicationManagerHandler Methods */

	@Override
	public void successHandler(final String result) {
		URL url;
		try {
			url = new URL(result);
			bmp = BitmapFactory.decodeStream(url.openConnection().getInputStream());
			//imageViewPressure.setImageBitmap(bmp);
			uiThreadHandler.post(new Runnable() {
		        @Override
		        public void run() {
		        	showErrorMessage("");
		        	loadInfectionPressureChart(result);
		        	hideProgressDialog();
		        }
		    });
		} catch (MalformedURLException badUrlException) {
			badUrlException.printStackTrace();
		} catch (IOException ioException) {
			ioException.printStackTrace();
		}
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
	        		stringErrorId = R.string.pressure_text_no_infection_to_display;
	        		break;
	        	default:
	        		stringErrorId = R.string.pressure_text_no_infection_to_display;
	        		break;
	        	}
	        	String errorMessage = getString(stringErrorId);
	        	showErrorMessage(errorMessage);
	        	hideProgressDialog();
	        }
	    });
	}
}