package com.af.euroblight2;

import java.io.File;
import java.io.IOException;
import java.lang.ref.WeakReference;
import java.util.ArrayList;

import com.af.euroblight2.R;
import com.af.euroblight2.handlers.ReportCommunicationManagerHandler;
import com.af.euroblight2.managers.CollectionsManager;
import com.af.euroblight2.managers.CommunicationManager;
import com.af.euroblight2.managers.DataManager;
import com.af.euroblight2.managers.ReportCommunicationManager;
import com.af.euroblight2.objects.CollectionItem;
import com.af.euroblight2.objects.Report;
import com.af.euroblight2.objects.ReportPhoto;

import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.MediaStore;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.res.Configuration;
import android.graphics.Bitmap;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import android.support.v4.app.NavUtils;

public class ReportActivity extends Activity implements OnItemSelectedListener, ReportCommunicationManagerHandler, LocationListener {
	
	static final int REQUEST_TAKE_PHOTO = 1;
	
	// GUI Variables
	ImageView imageViewPhoto;
	LinearLayout linearLayoutImage;
	LinearLayout linearLayoutCoordinate;
	Spinner spinnerTypeProduction;
	Spinner spinnerBBCH;
	Spinner spinnerSeverity;
	TextView textViewLatitude;
	TextView textViewLongitude;
	EditText editTextFieldName;
	EditText editTextVariety;
	
	// Variables 
	private final Handler uiThreadHandler = new Handler(Looper.getMainLooper());
	LocationManager locationManager; 
	ProgressDialog progressDialog;
	boolean isPhotoValid;
	boolean isLocationValid;
	
	// Adapters
	ArrayAdapter<?> spinnerTypeProductionAdapter;
	private final static Object TAG_TYPE_OF_PRODUCTION = new Object();
	ArrayAdapter<?> spinnerBBCHAdapter;
	private final static Object TAG_BBCH = new Object();
	ArrayAdapter<?> spinnerSeverityAdapter;
	private final static Object TAG_SEVERITY = new Object();
	
	/** Override Methods */
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_report);
		getWindow().setSoftInputMode(
			    WindowManager.LayoutParams.SOFT_INPUT_STATE_ALWAYS_HIDDEN);	
		spinnerBBCH = (Spinner)findViewById(R.id.spinnerBBCH);
		setupBBCHSpinner();
		spinnerSeverity = (Spinner)findViewById(R.id.spinnerSeverity);
		setupSeveritySpinner();
		spinnerTypeProduction = (Spinner)findViewById(R.id.spinnerSiteIds);
		setupTypeOfProductionSpinner();
		linearLayoutImage = (LinearLayout)findViewById(R.id.report_layout_image);
		imageViewPhoto = (ImageView)findViewById(R.id.imageViewReportPhoto);
		linearLayoutCoordinate = (LinearLayout)findViewById(R.id.report_layout_coordinate);
		locationManager = (LocationManager)getSystemService(Context.LOCATION_SERVICE);   
		textViewLatitude = (TextView)findViewById(R.id.textViewLatitudeValue);
		textViewLongitude = (TextView)findViewById(R.id.textViewLongitudeValue);
		editTextFieldName = (EditText)findViewById(R.id.editTextFieldName);
		editTextVariety = (EditText)findViewById(R.id.editTextVariety);		
		loadData();
		// Show the Up button in the action bar.
		setupActionBar();
	}
	
	@Override
	public void onConfigurationChanged(Configuration newConfig) {
	    super.onConfigurationChanged(newConfig);
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.report, menu);
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
		case R.id.menu_action_clear:
			actionClearCurrentReport();
			return true;
		}
		return super.onOptionsItemSelected(item);
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == REQUEST_TAKE_PHOTO) {
			Report currentReport = DataManager.sharedManager().getCurrentReport();
			if (resultCode == RESULT_OK) {
				// TODO : Active only if we need to show pictures in gallery
				sendBroadcast(currentReport.getPhoto().getMediaScanIntent());
				loadPhoto(currentReport);
			} else {
				currentReport.setPhoto(null);
			}
		}
	}
	
	/** Public Methods */
	
	public void takePhotoButtonTapped(View sender) {
		Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		// Ensure that there's a camera activity to handle the intent
		if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
			// Create the File where the photo should go
			File photoFile = null;
			try {
				Report currentReport = DataManager.sharedManager().getCurrentReport();
				currentReport.setPhoto(new ReportPhoto());
				photoFile = currentReport.getPhoto().createImageFile();
			} catch (IOException ex) {
				// Error occurred while creating the File
			}
			// Continue only if the File was successfully created
			if (photoFile != null) {
				takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT,
											Uri.fromFile(photoFile));
				startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
			}
		}
	}
	
	public void locationButtonTapped(View sender) { 
		  boolean gpsIsEnabled = locationManager
		            .isProviderEnabled(LocationManager.GPS_PROVIDER);
		  boolean networkIsEnabled = locationManager
		            .isProviderEnabled(LocationManager.NETWORK_PROVIDER) && 
		            CommunicationManager.isCommunicationPossible();
		 if (networkIsEnabled) {
		        locationManager.requestLocationUpdates(
		                LocationManager.NETWORK_PROVIDER, 0, 0,
		                (android.location.LocationListener) this);
		        showProgressDialog(getString(R.string.progress_dialod_please_wait_retrieve_coordinate), true);
		 } else if (gpsIsEnabled) {
		        locationManager.requestLocationUpdates(
		                LocationManager.GPS_PROVIDER, 0, 0,
		                (android.location.LocationListener) this);
		        showProgressDialog(getString(R.string.progress_dialod_please_wait_retrieve_coordinate_just_gps), true);
		} else {
			showAlertDialog(getString(R.string.alert_dialog_title_location_access),
							getString(R.string.alert_dialog_message_location_access_no_available),
							new DialogInterface.OnClickListener() {
					              public void onClick(DialogInterface dialog, int which) { 
					                  final Intent intent = new Intent(android.provider.Settings.ACTION_LOCATION_SOURCE_SETTINGS);
					                  startActivity(intent);
					              }
					        },
					        new DialogInterface.OnClickListener() {
					              public void onClick(DialogInterface dialog, int which) { 
					            	  // do nothing
					              }
					        }); 
		}
	}
	
	public void sendButtonTapped(View sender) {
		isPhotoValid = validatePhoto();
		isLocationValid = validateLocation();
		updateInputsValidationColor();
		if (isPhotoValid && isLocationValid) {
			if (!CommunicationManager.isCommunicationPossible()) {
				showAlertDialog(getString(R.string.alert_dialog_title_connection_access),
						getString(R.string.alert_dialog_message_connection_access_no_available),
						new DialogInterface.OnClickListener() {
				              public void onClick(DialogInterface dialog, int which) { 
				            	  addReportToUnsentReports();
				            	  clearCurrentReport(false);
				              }
				        },
				        new DialogInterface.OnClickListener() {
				              public void onClick(DialogInterface dialog, int which) { 
				            	  // do nothing
				              }
				        }); 
				return;
			}
			showProgressDialog(getString(R.string.progress_dialod_please_wait_while_sending_report), false);
			ReportCommunicationManager reportCommunicationManager = 
					new ReportCommunicationManager();
			String username = DataManager.sharedManager().storedUsername();
			String password = DataManager.sharedManager().storedPassword();
			Report currentReport = DataManager.sharedManager().getCurrentReport();
			currentReport.setFieldName(editTextFieldName.getText().toString());
			currentReport.setVariety(editTextVariety.getText().toString());
			reportCommunicationManager.uploadObservation(username, password, currentReport, this);
		}
	}
		
	/** Private Methods */

	/**
	 * Set up the {@link android.app.ActionBar}.
	 */
	private void setupActionBar() {
		getActionBar().setDisplayHomeAsUpEnabled(true);
	}
	
	private void setupBBCHSpinner() {
		CollectionsManager collectionsManager = CollectionsManager.sharedManager();
		final ArrayList<CollectionItem> collection = collectionsManager.getBBCHList();
		spinnerBBCHAdapter = setupArrayAdapterForCollection(collection);
		spinnerBBCH.setAdapter(spinnerBBCHAdapter);
		spinnerBBCH.setTag(TAG_BBCH);
		// Code to prevent the spinner from be fired twice 
		final WeakReference<ReportActivity> weakThis = new WeakReference<ReportActivity>(this);
		spinnerBBCH.post(new Runnable() {
		    public void run() {
		    	spinnerBBCH.setOnItemSelectedListener(weakThis.get());
		    }
		});
	}
	
	private void setupSeveritySpinner() {
		CollectionsManager collectionsManager = CollectionsManager.sharedManager();
		ArrayList<CollectionItem> collection = collectionsManager.getSeverityList();
		spinnerSeverityAdapter = setupArrayAdapterForCollection(collection);
		spinnerSeverity.setAdapter(spinnerSeverityAdapter);
		spinnerSeverity.setTag(TAG_SEVERITY);
		// Code to prevent the spinner from be fired twice 
		final WeakReference<ReportActivity> weakThis = new WeakReference<ReportActivity>(this);
		spinnerSeverity.post(new Runnable() {
		    public void run() {
		    	spinnerSeverity.setOnItemSelectedListener(weakThis.get());
		    }
		});
	}
	
	private void setupTypeOfProductionSpinner() {
		CollectionsManager collectionsManager = CollectionsManager.sharedManager();
		ArrayList<CollectionItem> collection = collectionsManager.getTypeOfProductionList();
		spinnerTypeProductionAdapter = setupArrayAdapterForCollection(collection);
		spinnerTypeProduction.setAdapter(spinnerTypeProductionAdapter);
		spinnerTypeProduction.setTag(TAG_TYPE_OF_PRODUCTION);
		// Code to prevent the spinner from be fired twice 
		final WeakReference<ReportActivity> weakThis = new WeakReference<ReportActivity>(this);
		spinnerTypeProduction.post(new Runnable() {
		    public void run() {
		    	spinnerTypeProduction.setOnItemSelectedListener(weakThis.get());
		    }
		});
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
	
	private void loadData() {
		Report currentReport = DataManager.sharedManager().getCurrentReport();
		loadFieldName(currentReport);
		loadSeverityValue(currentReport);
		loadBBCHValue(currentReport);
		loadTypeOfProduction(currentReport);
		loadVariety(currentReport);
		loadPhoto(currentReport);
		loadLocation(currentReport);
	}
	
	private void loadFieldName(Report report) {
		editTextFieldName.setText(report.getFieldName());
	}
	
	private void loadSeverityValue(Report report) {
		for(int i=0 ; i < spinnerSeverityAdapter.getCount() ; i++) {
			CollectionItem item = (CollectionItem)spinnerSeverityAdapter.getItem(i);
			if (item.getId() == report.getSeverityValue()) {
				spinnerSeverity.setSelection(i);
				return;
			}
		}
	}
	
	private void loadBBCHValue(Report report) {
		for(int i=0 ; i < spinnerBBCHAdapter.getCount() ; i++) {
			CollectionItem item = (CollectionItem)spinnerBBCHAdapter.getItem(i);
			if (item.getId() == report.getBBCHValue()) {
				spinnerBBCH.setSelection(i);
				return;
			}
		}
	}
	
	private void loadTypeOfProduction(Report report) {
		for(int i=0 ; i < spinnerTypeProductionAdapter.getCount() ; i++) {
			CollectionItem item = (CollectionItem)spinnerTypeProductionAdapter.getItem(i);
			if (item.getId() == report.getSelectedTypeOfProductionIndex()) {
				spinnerTypeProduction.setSelection(i);
				return;
			}
		}
	}
	
	private void loadVariety(Report report) {
		editTextVariety.setText(report.getVariety());
	}
	
	private void loadPhoto(Report report) {
		if (report.getPhoto() != null) {
			Bitmap imageBitmap = report.getPhoto().getBitmap();
			imageViewPhoto.setImageBitmap(imageBitmap);
			return;
		}
		imageViewPhoto.setImageResource(R.drawable.image_placeholder);
	}
	
	private void loadLocation(Report report) {
		textViewLatitude.setText(String.format("%f", report.getLatitudeValue()));
		textViewLongitude.setText(String.format("%f", report.getLongitudeValue()));
	}

	private void actionClearCurrentReport() {
		showAlertDialog(getString(R.string.alert_dialog_title_clear_report),
				getString(R.string.alert_dialog_message_clear_report),
				new DialogInterface.OnClickListener() {
		              public void onClick(DialogInterface dialog, int which) { 
		            	  clearCurrentReport(true);
		              }
		        },
		        new DialogInterface.OnClickListener() {
		              public void onClick(DialogInterface dialog, int which) { 
		            	  // fall through
		              }
		        }); 
	}
	
	private void clearCurrentReport(boolean shouldClearStoredReports) {
		DataManager.sharedManager().clearCurrentReport(shouldClearStoredReports);
		loadData();
		linearLayoutImage.setBackgroundResource(R.color.cell_default_input_color);
		linearLayoutCoordinate.setBackgroundResource(R.color.cell_default_input_color);
	}
	
	private void addReportToUnsentReports() {
		DataManager.sharedManager().storeCurrentReport();
		loadData();
	}
	
	private boolean validatePhoto() {
		Report currentReport = DataManager.sharedManager().getCurrentReport();
		return (currentReport.getPhoto() == null) ? false : true;
	}
	
	private boolean validateLocation() {
		Report currentReport = DataManager.sharedManager().getCurrentReport();
		return (currentReport.getLongitudeValue() == 0.0 && 
				currentReport.getLatitudeValue() == 0.0) ? false : true;
	}
	
	private void updateInputsValidationColor() {
		linearLayoutImage.setBackgroundResource(
				(isPhotoValid) ? R.color.cell_default_input_color : 
									  R.color.cell_invalid_input_color);
		linearLayoutCoordinate.setBackgroundResource(
				(isLocationValid) ? R.color.cell_default_input_color : 
									  	 R.color.cell_invalid_input_color);
	}

	private void showProgressDialog(final String message, boolean cancelable) {
		hideProgressDialog();
		if (progressDialog == null) {
			progressDialog = new ProgressDialog(this);
		}
		progressDialog.setCancelable(cancelable);
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
				if (progressDialog != null && progressDialog.isShowing()) {
					progressDialog.cancel();	
				}
			}
		});
	}
	
	private void showAlertDialog(String title, 
							   	 String message,
							     DialogInterface.OnClickListener positiveListener, 
							     DialogInterface.OnClickListener negativeListener) { 
        new AlertDialog.Builder(this)
        .setTitle(title)
        .setMessage(message)
        .setPositiveButton(android.R.string.yes, positiveListener)
        .setNegativeButton(android.R.string.no, negativeListener)
        .setIcon(android.R.drawable.ic_dialog_alert)
        .show();
	}
	
	private void showToastMessage(String message) {
		Toast.makeText(getApplicationContext(), 
					   message, 
					   Toast.LENGTH_LONG).show();
	}

	/** OnItemSelectedListener Methods */
	
	@Override
	public void onItemSelected(AdapterView<?> adapter, View view, int position, long i) {
		Report currentReport = DataManager.sharedManager().getCurrentReport();
		CollectionItem item;
		if (adapter.getTag() == TAG_TYPE_OF_PRODUCTION) {
			item = (CollectionItem)spinnerTypeProductionAdapter.getItem(position);
			currentReport.setSelectedTypeOfProductionIndex(item.getId());
		} else if (adapter.getTag() == TAG_BBCH) {
			item = (CollectionItem)spinnerBBCHAdapter.getItem(position);
			currentReport.setBBCHValue(item.getId());
		} else if (adapter.getTag() == TAG_SEVERITY) {
			item = (CollectionItem)spinnerSeverityAdapter.getItem(position);
			currentReport.setSeverityValue(item.getId());
		}
	}

	@Override
	public void onNothingSelected(AdapterView<?> arg0) {
		// fall through
	}
	
	/** CommunicationManagerHandler Methods */
	
	@Override
	public void successHandler(final Report report,final String result) {
		uiThreadHandler.post(new Runnable() {
	        @Override
	        public void run() {
	        	hideProgressDialog();
	        	clearCurrentReport(true);
	        	String successMessage = getString(R.string.success_text_current_report_send);
	        	showToastMessage(successMessage);
	        }
	    });
	}
	
	@Override
	public void successHandler(final String result) {
		// fall through
	}

	@Override
	public void errorHandler(final ERRORS_DEFINITION_CODE errorCode) {
		// fall through
	}
	
	@Override
	public void errorHandler(final Report report, final ERRORS_DEFINITION_CODE errorCode) {
		uiThreadHandler.post(new Runnable() {
	        @Override
	        public void run() {
	        	hideProgressDialog();
	        	addReportToUnsentReports();
	        	int stringErrorId = 
	        			(errorCode == ERRORS_DEFINITION_CODE.ERROR_UNAUTHORIZED) ?
	        					R.string.error_text_unauthorized : R.string.error_text_error_while_sending_report;
	        	String errorMessage = getString(stringErrorId);
	        	showToastMessage(errorMessage);
	        }
	    });
	}
	
	/** LocationListener Methods */
	
	@Override
	public void onLocationChanged(Location location) {
		locationManager.removeUpdates(this); 
		Report currentReport = DataManager.sharedManager().getCurrentReport();
		currentReport.setLatitudeValue(location.getLatitude());
		currentReport.setLongitudeValue(location.getLongitude());
		loadLocation(currentReport);
		hideProgressDialog();
	}

	@Override
	public void onProviderDisabled(String arg0) {
		locationManager.removeUpdates(this); 
		Report currentReport = DataManager.sharedManager().getCurrentReport();
		currentReport.setLatitudeValue(0.0);
		currentReport.setLongitudeValue(0.0);
		loadLocation(currentReport);
		hideProgressDialog();
	}

	@Override
	public void onProviderEnabled(String arg0) {
		hideProgressDialog();
	}

	@Override
	public void onStatusChanged(String arg0, int arg1, Bundle arg2) {
		// fall through
	}
}