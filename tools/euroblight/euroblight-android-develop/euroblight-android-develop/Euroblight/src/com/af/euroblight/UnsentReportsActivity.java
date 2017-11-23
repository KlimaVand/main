package com.af.euroblight;

import java.util.ArrayList;
import java.util.HashMap;

import com.af.euroblight.adapters.UnsentReportsListAdapter;
import com.af.euroblight.handlers.ReportCommunicationManagerHandler;
import com.af.euroblight.managers.DataManager;
import com.af.euroblight.managers.ReportCommunicationManager;
import com.af.euroblight.objects.Report;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;
import android.widget.Toast;
import android.support.v4.app.NavUtils;

public class UnsentReportsActivity extends Activity implements ReportCommunicationManagerHandler  {

	// Variables
	private String progressDialogUnsentReportsMessage;
	private int countOfReportsToSend;
	private ListView listView;
	private UnsentReportsListAdapter unsentReportsListAdapter;
	private final Handler uiThreadHandler = new Handler(Looper.getMainLooper());
	private HashMap<Report, ReportCommunicationManager> onGoingCommunicationManagers = new HashMap<Report, ReportCommunicationManager>(); 
	ProgressDialog progressDialog;
	
	/** Override Methods */
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_unsent_reports);
		listView = (ListView)findViewById(R.id.listOfUnsentReports);
		progressDialogUnsentReportsMessage = getString(R.string.progress_dialog_unsent_reports_progress);
		// Show the Up button in the action bar.
		setupActionBar();
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		new Handler().postDelayed(new Runnable() {
			@Override
		    public void run() {
				configureListView();
		    }
		}, 100); // starting it in 0.1 second
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.unsent_reports, menu);
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
			actionClearAllUnsentReports();
			return true;
		case R.id.menu_action_send:
			actionSendAllUnsentReports();
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
	
	private void configureListView() {
		ArrayList<Report> unsentReports = DataManager.sharedManager().storedReports();
		unsentReportsListAdapter = new UnsentReportsListAdapter(this, 
				R.layout.activity_unsent_reports_list_row, unsentReports); 
		listView.setAdapter(unsentReportsListAdapter);
	}
	
	private void actionClearAllUnsentReports() {
		// Return if there is no unsentReports
		if (unsentReportsListAdapter.isEmpty()) {
			return;
		}
		showAlertDialog(getString(R.string.alert_dialog_title_clear_reports),
				getString(R.string.alert_dialog_message_clear_all_reports),
				new DialogInterface.OnClickListener() {
		              public void onClick(DialogInterface dialog, int which) { 
		            	  DataManager.sharedManager().clearAllStoredReports();
		            	  unsentReportsListAdapter.clear();
		            	  unsentReportsListAdapter.notifyDataSetChanged();
		              }
		        },
		        new DialogInterface.OnClickListener() {
		              public void onClick(DialogInterface dialog, int which) { 
		            	  // fall through
		              }
		        }); 
	}
	
	private void actionSendAllUnsentReports() {
		// Return if there is no unsentReports
		if (unsentReportsListAdapter.isEmpty()) {
			return;
		}
		showAlertDialog(getString(R.string.alert_dialog_title_send_reports),
				getString(R.string.alert_dialog_message_send_all_reports),
				new DialogInterface.OnClickListener() {
		              public void onClick(DialogInterface dialog, int which) { 
		            	  sendAllUnsentReports();
		              }
		        },
		        new DialogInterface.OnClickListener() {
		              public void onClick(DialogInterface dialog, int which) { 
		            	// fall through
		              }
		        }); 
	}
	
	private void sendAllUnsentReports() {
		if (!ReportCommunicationManager.isCommunicationPossible()) {
			showAlertDialog(getString(R.string.alert_dialog_title_connection_access),
							getString(R.string.alert_dialog_message_connection_access_no_available_try_later),
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int which) { 
									// Close this Activity
									finish();
								}
							},
							new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int which) { 
									// fall through
								}
							}); 
			return;
		}
		showProgressDialog(getString(R.string.progress_dialod_please_wait_while_sending_report_progress));
		countOfReportsToSend = unsentReportsListAdapter.getCount();
		for (int i = 0; i < unsentReportsListAdapter.getCount(); i++) {
			 ReportCommunicationManager reportCommunicationManager = 
						new ReportCommunicationManager();
				String username = DataManager.sharedManager().storedUsername();
				String password = DataManager.sharedManager().storedPassword();
				Report report = unsentReportsListAdapter.getItem(i);
				reportCommunicationManager.uploadObservation(username, password, report, this);
		 }
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
	
	private void showProgressDialog(final String message) {
		hideProgressDialog();
		if (progressDialog == null) {
			progressDialog = new ProgressDialog(this);
			progressDialog.setCancelable(false);
		}
		uiThreadHandler.post(new Runnable() {
			@Override
			public void run() {
				updateProgressDialogMessage(message);
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
	
	private void updateProgressDialogMessage(final String message) {
		uiThreadHandler.post(new Runnable() {
			@Override
			public void run() {
				if (progressDialog != null && progressDialog.isShowing()) {
					String progressMessage = String.format(message+progressDialogUnsentReportsMessage,
														   countOfReportsToSend - unsentReportsListAdapter.getCount(), 
														   countOfReportsToSend);
					progressDialog.setMessage(progressMessage);
				}
			}
		});
	}
	
	private void removeSentReport(Report report, boolean success) {
		if (success) {
			unsentReportsListAdapter.remove(report);
			DataManager.sharedManager().clearStoredReport(report);
		}
		onGoingCommunicationManagers.remove(report);
		updateProgressDialogMessage(getString(R.string.progress_dialod_please_wait_while_sending_report_progress));
		unsentReportsListAdapter.notifyDataSetChanged();
		if (onGoingCommunicationManagers.size() == 0) {
    		hideProgressDialog();
    		int resurseId = (unsentReportsListAdapter.isEmpty()) ? 
    				R.string.success_text_current_report_send :
    				R.string.error_text_error_while_sending_report;
    		
    		showToastMessage(getString(resurseId));
    	}
	}
	
	private void showToastMessage(String message) {
		Toast.makeText(getApplicationContext(), 
					   message, 
					   Toast.LENGTH_LONG).show();
	}
	
	/** CommunicationManagerHandler Methods */
	
	@Override
	public void successHandler(final Report report,final String result) {
		uiThreadHandler.post(new Runnable() {
			@Override
			public void run() {
				removeSentReport(report, true); 
			}
		});
	}
	
	@Override
	public void successHandler(final String result) {
		// fall through
	}

	@Override
	public void errorHandler(ERRORS_DEFINITION_CODE errorCode) {
		// fall through
	}
	
	@Override
	public void errorHandler(final Report report, final ERRORS_DEFINITION_CODE errorCode) {
		uiThreadHandler.post(new Runnable() {
			@Override
			public void run() {
				if	(errorCode == ERRORS_DEFINITION_CODE.ERROR_UNAUTHORIZED) {
					String errorMessage = getString(R.string.error_text_unauthorized);
					hideProgressDialog();
					onGoingCommunicationManagers.clear();
					showToastMessage(errorMessage);
					return;
				}
				removeSentReport(report, false); 
			}
		});
	}

}