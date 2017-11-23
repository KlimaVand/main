package com.af.euroblight2.objects;

import java.io.Serializable;
import java.util.Date;

public class Report implements Serializable {

	/** Serializable Id */
	
	private static final long serialVersionUID = 1L;

	// Variables
	Date createdDate;
	double latitudeValue;
	double longitudeValue;
	String locationName;
	String surveyorName;
	String institutionName;
	String fieldName;
	String variety;
	ReportPhoto photo;
	int selectedTypeOfProductionIndex;
	int severityValue;
	int BBCHValue;
	
	public Report() {
		super();
		createdDate = new Date();
		latitudeValue = 0.0;
		longitudeValue = 0.0;
        severityValue = 15;
        BBCHValue = 15;
        selectedTypeOfProductionIndex = -9;
        severityValue = -9;
        BBCHValue = -9;
        locationName = "";
        surveyorName = "";
        institutionName = "";
        fieldName = "";
    	variety = "";
	}

	public Date getCreatedDate() {
		return createdDate;
	}

	public double getLatitudeValue() {
		return latitudeValue;
	}

	public void setLatitudeValue(double latitudeValue) {
		this.latitudeValue = latitudeValue;
	}

	public double getLongitudeValue() {
		return longitudeValue;
	}

	public void setLongitudeValue(double longitudeValue) {
		this.longitudeValue = longitudeValue;
	}

	public ReportPhoto getPhoto() {
		return photo;
	}

	public void setPhoto(ReportPhoto photo) {
		this.photo = photo;
	}

	public int getSelectedTypeOfProductionIndex() {
		return selectedTypeOfProductionIndex;
	}

	public void setSelectedTypeOfProductionIndex(int selectedTypeOfProductionIndex) {
		this.selectedTypeOfProductionIndex = selectedTypeOfProductionIndex;
	}

	public int getSeverityValue() {
		return severityValue;
	}

	public void setSeverityValue(int severityValue) {
		this.severityValue = severityValue;
	}

	public int getBBCHValue() {
		return BBCHValue;
	}

	public void setBBCHValue(int bBCHValue) {
		BBCHValue = bBCHValue;
	}
	
	public String getFieldName() {
		return fieldName;
	}

	public void setFieldName(String name)  {
		fieldName = name;
	}
	
	public String getVariety() {
		return variety;
	}

	public void setVariety(String varietyValue)  {
		variety = varietyValue;
	}
}