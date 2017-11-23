package com.af.euroblight2.managers;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.ArrayList;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;

import com.af.euroblight2.AppDelegate;
import com.af.euroblight2.objects.Report;

public class DataManager {
	
	static final String FILE_NAME_REPORTS = "reports";
	
	// Variables
	private static DataManager sharedManager; 
	private SharedPreferences preferences;
	private Report currentReport;
	
	// Keys
	private final String USER_PREFERENCES = "EB_USER_PREFERENCES";
	private final String  USERNAME = "EB_USERNAME";
	private final String  PASSWORD = "EB_PASSWORD";
		
	public static DataManager sharedManager() {
		if (sharedManager == null) {
			sharedManager = new DataManager();
		}
		return sharedManager;
	}	
	
	public void storeUserCredentials(String username, String password) {
		SharedPreferences.Editor preferensEditor = preferences.edit();
		preferensEditor.putString(USERNAME, username);
		preferensEditor.putString(PASSWORD, password);
		preferensEditor.commit();
	}	
	
	public boolean isStoredUserCredentialsSufficent() {
		return storedUsername() != null && storedPassword() != null;
	}
	
	public String storedUsername() {
		return preferences.getString(USERNAME, null);
	}
	
	public String storedPassword() {
		return preferences.getString(PASSWORD, null);
	}
	
	public Report getCurrentReport() {
		return currentReport;
	}
	
	public void storeCurrentReport() {
		ArrayList<Report> storedReports = storedReports();
		if (storedReports.size() == 0) {
			addCurrentReportToStoredReports(storedReports);
			return;
		}
		boolean shouldAddReport = true;
		for (Report report : storedReports) {
			if (isCurrentReportEqualToReport(report)) {
				shouldAddReport = false;
				break;
			}
		}
		if (shouldAddReport) {
			addCurrentReportToStoredReports(storedReports);
		}
	}
	
	public void clearCurrentReport(boolean shouldClearStoredReports) {
		if (shouldClearStoredReports) {
			clearStoredReport(currentReport);
		}
		currentReport = new Report();
	}
	
	public void clearStoredReport(Report reportToClear) {
		ArrayList<Report> storedReports = deserializeStoredReports();
		int index = 0;
		boolean shouldRemoveCurrentReport = false;
		for (Report report : storedReports) {
			boolean isDateEqual = report.getCreatedDate().compareTo(reportToClear.getCreatedDate()) == 0;
			if(isDateEqual) {
				shouldRemoveCurrentReport = true;
		        break;
		    }
		    index++;
		}
		if(shouldRemoveCurrentReport) {
			storedReports.remove(index);
			serializeStoredReports(storedReports);
		}
	    serializeStoredReports(storedReports);
	}
	
	public void clearAllStoredReports() {
		AppDelegate.getContext().deleteFile(FILE_NAME_REPORTS);
	}
	
	public ArrayList<Report> storedReports() {
		ArrayList<Report> storedReports = deserializeStoredReports();
		if (storedReports == null) {
			storedReports = new ArrayList<Report>();
			serializeStoredReports(storedReports);
		}
		return storedReports;
	}
	
	public boolean hasStoredReports() {
		ArrayList<Report> storedReports = deserializeStoredReports();
		if (storedReports == null || storedReports.isEmpty()) {
			return false;
		}
		return true;
	}
	
	public void removeSentReportFromStoredReportsIfNeeded() {
		ArrayList<Report> storedReports = storedReports();
		int index = 0;
		boolean shouldRemoveCurrentReport = false;
		for (Report report : storedReports) {
			if(isCurrentReportEqualToReport(report)) {
				shouldRemoveCurrentReport = true;
		        break;
		    }
		    index++;
		}
		if(shouldRemoveCurrentReport) {
			storedReports.remove(index);
			serializeStoredReports(storedReports);
		}
	}
		
	protected DataManager() {
		super();
		currentReport = new Report();
		preferences = AppDelegate.getContext().getSharedPreferences(USER_PREFERENCES, 
																	Activity.MODE_PRIVATE);
	}
	
	private void addCurrentReportToStoredReports(ArrayList<Report> storedReports) {
		storedReports.add(currentReport);
		serializeStoredReports(storedReports);
	}
	
	private boolean isCurrentReportEqualToReport(Report report) {
		boolean isDateEqual = currentReport.getCreatedDate().compareTo(report.getCreatedDate()) == 0;
		boolean isLatitudeEqual = currentReport.getLatitudeValue() == report.getLatitudeValue();
		boolean isLongitudeEqual = currentReport.getLongitudeValue() == report.getLongitudeValue();
		return isDateEqual && isLatitudeEqual && isLongitudeEqual;
	}
	
	private void serializeStoredReports(ArrayList<Report> storedReports) {
		try{
			FileOutputStream fileOutputStream = AppDelegate.getContext().openFileOutput(FILE_NAME_REPORTS, 
																					Context.MODE_PRIVATE);
			ObjectOutputStream objectOutputStream = new ObjectOutputStream(fileOutputStream);
			objectOutputStream.writeObject(storedReports);
			objectOutputStream.close();
			fileOutputStream.close();
		} catch(IOException ioException) {
			ioException.printStackTrace();
		}
	}
	
	@SuppressWarnings("unchecked")
	private ArrayList<Report> deserializeStoredReports() {
		try {
			FileInputStream fileInputStream = AppDelegate.getContext().openFileInput(FILE_NAME_REPORTS);
			ObjectInputStream objectInputStream = new ObjectInputStream(fileInputStream);
			ArrayList<Report> storedReports;
			storedReports = (ArrayList<Report>)objectInputStream.readObject();
			objectInputStream.close();
			fileInputStream.close();
			return storedReports;
		}catch(IOException ioException){
			ioException.printStackTrace();
			return null;
		}catch(ClassNotFoundException classNotFoundException){
			System.out.println("Class not found");
			classNotFoundException.printStackTrace();
			return null;
		}
	}
}