package com.af.euroblight2.managers;

import java.io.ByteArrayOutputStream;
import java.text.SimpleDateFormat;
import java.util.Locale;

import org.apache.http.HttpEntity;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.impl.client.DefaultHttpClient;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;

import com.af.euroblight2.handlers.ReportCommunicationManagerHandler;
import com.af.euroblight2.objects.Report;
import com.af.euroblight2.handlers.CommunicationManagerHandler;

@SuppressLint("SimpleDateFormat")
public class ReportCommunicationManager extends CommunicationManager {
	
	// API
	static final String KEY_LATITUDE = "aLatitude";
	static final String KEY_LONGITUDE = "aLongitude";
	static final String KEY_SURVEY_SITE_ID = "aSurveySiteID";
	static final String KEY_SURVEY_NAME = "aSurveyorName";
	static final String KEY_INSTITUTION_NAME = "aInstitutionName";
	static final String KEY_SEVERITY_ID = "aSeverityID";
	static final String KEY_GROWTH_STAGE_ID = "aGrowthStageID";
	static final String KEY_IMAGE = "aImage";
	static final String KEY_FIELD_NAME = "aFieldName";
	static final String KEY_VARIETY = "aVariety";
	static final String KEY_OBSERVATION_DATE = "aObservationDate";

	static final String REPORT_DATE_FORMAT = "MM-dd-yyyy";
	
	/** Public Methods */
	
	public void uploadObservation(String username, 
	   		  					  String password,
	   		  					  Report report,
	   		  					  ReportCommunicationManagerHandler handler){
		ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
		report.getPhoto().getBitmap().compress(Bitmap.CompressFormat.PNG, 75, outputStream);
		byte[] imageData = outputStream.toByteArray();
		
		/* Reset ImageData to nil for maximum string content length error
		 * The maximum string content length quota (8192) has been exceeded while reading XML data. 
		 * This quota may be increased by changing the MaxStringContentLength property on the 
		 * XmlDictionaryReaderQuotas object used when creating the XML reader.
		 * Line 15, position 12089.
		 */
		/*imageData = null;*/
		String soapMessage = String.format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
				SOAP_HEADER_BASE,
				SOAP_ENVELOPE_START,
                SOAP_BODY_START,
                String.format(SOAP_BODY_ACTION_START,SOAP_ACTION_UPLOAD_OBSERVATION),
                String.format(SOAP_BODY_PARAMETER,KEY_LOGIN,username,KEY_LOGIN),
                String.format(SOAP_BODY_PARAMETER,KEY_PASSWORD,password,KEY_PASSWORD),
                String.format(SOAP_BODY_PARAMETER,KEY_LATITUDE,
                		String.format(Locale.ENGLISH, "%f", report.getLatitudeValue()),
                		KEY_LATITUDE),
                String.format(SOAP_BODY_PARAMETER,KEY_LONGITUDE,
                		String.format(Locale.ENGLISH, "%f", report.getLongitudeValue()),
                		KEY_LONGITUDE),
                String.format(SOAP_BODY_PARAMETER,KEY_LOCATION_NAME,"",KEY_LOCATION_NAME),
                String.format(SOAP_BODY_PARAMETER,KEY_SURVEY_SITE_ID,String.format("%d",report.getSelectedTypeOfProductionIndex()),KEY_SURVEY_SITE_ID),
                String.format(SOAP_BODY_PARAMETER,KEY_SURVEY_NAME,"",KEY_SURVEY_NAME),
                String.format(SOAP_BODY_PARAMETER,KEY_INSTITUTION_NAME,"",KEY_INSTITUTION_NAME),
                String.format(SOAP_BODY_PARAMETER,KEY_GROWTH_STAGE_ID,String.format("%d",report.getBBCHValue()),KEY_GROWTH_STAGE_ID),
                String.format(SOAP_BODY_PARAMETER,KEY_SEVERITY_ID,String.format("%d",report.getSeverityValue()),KEY_SEVERITY_ID),
                String.format(SOAP_BODY_PARAMETER,KEY_IMAGE,String.format("%s",encodeToBase64(imageData)),KEY_IMAGE),
                String.format(SOAP_BODY_PARAMETER,KEY_FIELD_NAME,report.getFieldName(),KEY_FIELD_NAME),
                String.format(SOAP_BODY_PARAMETER,KEY_VARIETY,report.getVariety(),KEY_VARIETY),
                String.format(SOAP_BODY_PARAMETER,KEY_OBSERVATION_DATE,
                		new SimpleDateFormat(REPORT_DATE_FORMAT).format(report.getCreatedDate()),
                		KEY_OBSERVATION_DATE), 
                String.format(SOAP_BODY_ACTION_END,SOAP_ACTION_UPLOAD_OBSERVATION),
                SOAP_BODY_END,
                SOAP_ENVELOPE_END
				);
		executeRequest(report,soapMessage, SOAP_ACTION_UPLOAD_OBSERVATION, handler);
	}
	
	/** Private Methods */
	
	private void executeRequest(final Report report, 
			final String soapMessage, 
			final String soapAction, 
			final ReportCommunicationManagerHandler handler) {
		Thread executionRequestThread = new Thread(new Runnable() {
			public void run() {
				final DefaultHttpClient httpClient = new DefaultHttpClient();
				configureHttpClient(httpClient);
				String response = "";
				try { 
					// POST the envelope
					HttpPost httpPost = new HttpPost(SOAP_SERVICE_URL);
					// add headers
					HttpEntity entity = new StringEntity(convertStringToUTF8(soapMessage));
					System.out.println(soapMessage);
					httpPost.setHeader("soapaction", String.format("%s%s", SOAP_ACTION_BASE, soapAction));
					httpPost.setHeader("Content-Type", "text/xml; charset=utf-8");
					httpPost.setEntity(entity);
					ResponseHandler<String> responseHandler2 = new BasicResponseHandler();
					response = httpClient.execute(httpPost, responseHandler2);  
				}catch (Exception e) {
					if (handler != null) {
						System.out.println("Error: "+e.getMessage());
						handler.errorHandler(report , CommunicationManagerHandler.ERRORS_DEFINITION_CODE.COMMUNICATION_ERROR);
					}
				}
				// close the connection
				httpClient.getConnectionManager().shutdown();
				// handle the response
				handleResponse(report, response, soapAction, handler); 
			}
		});
		executionRequestThread.start();
	}
	
	private void handleResponse(Report report, 
			String response, 
			String soapAction, 
			ReportCommunicationManagerHandler handler) {
		Document document = null;
		try {
			String resultValue = "";
			document = convertStringToDoc(response);
			NodeList actionResultNodeList =  document.getElementsByTagName(String.format("%sResult", soapAction));
			for (int actionResultNodeIndex = 0; actionResultNodeIndex < actionResultNodeList.getLength(); actionResultNodeIndex++) {
				Element elementResult = (Element)actionResultNodeList.item(actionResultNodeIndex);
				NodeList elementResultNodeList = elementResult.getChildNodes();
				for (int elementResultNodeIndex = 0; elementResultNodeIndex < elementResultNodeList.getLength(); elementResultNodeIndex++) {
					resultValue = elementResultNodeList.item(elementResultNodeIndex).getNodeValue();
				}
			}
			if (resultValue.isEmpty() || handler == null) {
				throw new Exception();
			}
			int resultCode = Integer.parseInt(resultValue);
			if (resultCode == 20) {
				handler.successHandler(report, resultValue);
			} else {
				handler.errorHandler(report, CommunicationManagerHandler.ERRORS_DEFINITION_CODE.getCode(resultCode));
			} 
		} 
		catch (Exception exception) {
			if (handler != null) {
				handler.errorHandler(report, CommunicationManagerHandler.ERRORS_DEFINITION_CODE.PARSE_ERROR);
			}
		}
	}
}