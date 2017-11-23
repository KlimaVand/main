package com.af.euroblight2.managers;

import java.io.IOException;
import java.io.StringReader;
import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.params.HttpProtocolParams;
import org.apache.http.util.EntityUtils;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.InputSource;

import com.af.euroblight2.AppDelegate;
import com.af.euroblight2.handlers.CommunicationManagerHandler;
import com.af.euroblight2.handlers.CommunicationManagerHandler.ERRORS_DEFINITION_CODE;

import android.annotation.SuppressLint;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.util.Base64;

@SuppressLint("SimpleDateFormat")
public class CommunicationManager {
		  								 
	// Soap base
	static final String SOAP_HEADER_BASE = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	static final String SOAP_ENVELOPE_START = "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ns1=\"http://tempuri.org/\">\n";
	static final String SOAP_BODY_START = "<SOAP-ENV:Body>\n";
	static final String SOAP_BODY_ACTION_START = "<ns1:%s>\n";
	static final String SOAP_BODY_PARAMETER = "<ns1:%s>%s</ns1:%s>\n";
	static final String SOAP_BODY_ACTION_END = "</ns1:%s>\n";
	static final String SOAP_BODY_END = "</SOAP-ENV:Body>\n";
	static final String SOAP_ENVELOPE_END = "</SOAP-ENV:Envelope>";
	static final String SOAP_ACTION_BASE = "http://tempuri.org/IServiceSurvey/";

	// API
	static final String SOAP_ACTION_UPLOAD_OBSERVATION = "UploadObservation";
	static final String SOAP_ACTION_UPLOAD_OBSERVATION2 = "UploadObservation2";
	static final String SOAP_ACTION_MAP_KML_FILE = "MapKMLFile";
	static final String SOAP_ACTION_INFECTION_PRESSURE = "InfectionPressure";
	static final String SOAP_SERVICE_URL = "http://130.226.173.136/ProjectNET/Service/SurveyPLB/ServiceSurvey.svc";
	static final String KEY_LOGIN = "aLoginName";
	static final String KEY_PASSWORD = "aPassWord";
	static final String KEY_COUNTRY_ID = "aCountryID";
	static final String KEY_LOCATION_NAME = "aLocationName";
	static final String KEY_LOCATION_COORDINATE = "aLocationCoordinate";
	static final String KEY_WIDTH = "aWidth";
	static final String KEY_HEIGHT = "aHeight";

	/** Public Methods */
	
	public static boolean isCommunicationPossible() {
		ConnectivityManager connectivityManager = 
				(ConnectivityManager) AppDelegate.getDelegate().getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo networkInfo = connectivityManager.getActiveNetworkInfo();
		return  (networkInfo == null) ? false : true;
	} 

	public void retrieveKMLFile(String username,
						 		String password,
						 		String countryId,
						 		CommunicationManagerHandler handler) {
		String soapMessage = String.format("%s%s%s%s%s%s%s%s%s%s",
				SOAP_HEADER_BASE,
				SOAP_ENVELOPE_START,
                SOAP_BODY_START,
                String.format(SOAP_BODY_ACTION_START, SOAP_ACTION_MAP_KML_FILE),
                String.format(SOAP_BODY_PARAMETER,KEY_LOGIN,username,KEY_LOGIN),
                String.format(SOAP_BODY_PARAMETER,KEY_PASSWORD,password,KEY_PASSWORD),
                String.format(SOAP_BODY_PARAMETER,KEY_COUNTRY_ID,countryId,KEY_COUNTRY_ID),
                String.format(SOAP_BODY_ACTION_END, SOAP_ACTION_MAP_KML_FILE),
                SOAP_BODY_END,
                SOAP_ENVELOPE_END
				);
		executeRequest(soapMessage, SOAP_ACTION_MAP_KML_FILE, handler);
	}

	public void retrieveInfectionPressure(String username,
              					   		  String password,
              					   		  String locationName,
              					   		  String locationCoordinate,
              					   		  int width,
              					   		  int height,
              					   		  CommunicationManagerHandler handler) {
		String soapMessage = String.format("%s%s%s%s%s%s%s%s%s%s%s%s%s",
				SOAP_HEADER_BASE,
				SOAP_ENVELOPE_START,
                SOAP_BODY_START,
                String.format(SOAP_BODY_ACTION_START, SOAP_ACTION_INFECTION_PRESSURE),
                String.format(SOAP_BODY_PARAMETER,KEY_LOGIN,username,KEY_LOGIN),
                String.format(SOAP_BODY_PARAMETER,KEY_PASSWORD,password,KEY_PASSWORD),
                String.format(SOAP_BODY_PARAMETER,KEY_LOCATION_NAME,locationName,KEY_LOCATION_NAME),
                String.format(SOAP_BODY_PARAMETER,KEY_LOCATION_COORDINATE,locationCoordinate,KEY_LOCATION_COORDINATE),
                String.format(SOAP_BODY_PARAMETER,KEY_WIDTH,String.format("%d",width),KEY_WIDTH),
                String.format(SOAP_BODY_PARAMETER,KEY_HEIGHT,String.format("%d",height),KEY_HEIGHT),
                String.format(SOAP_BODY_ACTION_END, SOAP_ACTION_INFECTION_PRESSURE),
                SOAP_BODY_END,
                SOAP_ENVELOPE_END
				);
		executeRequest(soapMessage, SOAP_ACTION_INFECTION_PRESSURE, handler);
    }
	
	/** Protected Methods */
	
	protected void configureHttpClient(DefaultHttpClient httpClient) {
		// request parameters 
		HttpParams httpParams = httpClient.getParams();
		HttpConnectionParams.setConnectionTimeout(httpParams, 10000);
		HttpConnectionParams.setSoTimeout(httpParams, 15000);
    	// set parameter
    	HttpProtocolParams.setUseExpectContinue(httpClient.getParams(), true);
	}

	protected String encodeToBase64(byte[] data) {
		String base64Data = "";
		if (data != null) {
			// get the base 64 string
			base64Data = Base64.encodeToString(data, Base64.NO_WRAP);
		}
		return base64Data;
	}
	
    protected Document convertStringToDoc(String stringToConvert) throws Exception {
 		StringReader stringReader = new StringReader(stringToConvert);
 		InputSource inputSource = new InputSource(stringReader);
 		DocumentBuilderFactory documentBuilderFactory = DocumentBuilderFactory.newInstance();
 		DocumentBuilder documentBuilder = documentBuilderFactory.newDocumentBuilder();
 		Document document = documentBuilder.parse(inputSource);
 		stringReader.close();
 		document.getDocumentElement().normalize();
 		return document;
 	}
    
    protected String convertStringToUTF8(String stringToConvert) {
    	String stringUTF8 = "";
		try {
			stringUTF8 =  new String(stringToConvert.getBytes("UTF-8"), "ISO-8859-1");
		} catch (UnsupportedEncodingException e1) {
			e1.printStackTrace();
		}
		return stringUTF8;
    }
	
	/** Private Methods */
	
	private void executeRequest(final String soapMessage, 
							   	final String soapAction, 
							   	final CommunicationManagerHandler handler) {
		Thread executionRequestThread = new Thread(new Runnable() {
		    public void run() {
		    	final DefaultHttpClient httpClient = new DefaultHttpClient();
				configureHttpClient(httpClient);
				String response = "";
				try { 
			    	// POST the envelope
			    	HttpPost httpPost = new HttpPost(SOAP_SERVICE_URL);
			    	// add headers
			    	httpPost.setHeader("soapaction", String.format("%s%s", SOAP_ACTION_BASE, soapAction));
			    	httpPost.setHeader("Content-Type", "text/xml; charset=utf-8");
		    		HttpEntity entity = new StringEntity(convertStringToUTF8(soapMessage));
		    		httpPost.setEntity(entity);
		    		response = httpClient.execute(httpPost, responseHandler);  
				}catch (Exception e) {
					if (handler != null) {
						handler.errorHandler(ERRORS_DEFINITION_CODE.COMMUNICATION_ERROR);
					}
		    	}
				// close the connection
		    	httpClient.getConnectionManager().shutdown();
		    	// handle the response
		    	handleResponse(response, soapAction, handler); 
		    }
		});
		executionRequestThread.start();
	}
	
	private void handleResponse(String response, String soapAction, CommunicationManagerHandler handler) {
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
			if (this.validateURL(resultValue)) {
				handler.successHandler(resultValue);
				return;
			} 
			int resultCode = Integer.parseInt(resultValue);
			if (resultCode == ERRORS_DEFINITION_CODE.ERROR_SERVER_FAILED.getNumericType() || 
					resultCode == ERRORS_DEFINITION_CODE.ERROR_UNAUTHORIZED.getNumericType()) {
				handler.errorHandler(ERRORS_DEFINITION_CODE.getCode(resultCode));
				return;
			}
			throw new Exception();
		} 
		catch (Exception exception) {
			if (handler != null) {
				handler.errorHandler(ERRORS_DEFINITION_CODE.PARSE_ERROR);
			}
		}
	}
		
	private boolean validateURL(String url) {
		try {
			// try to create an URL object with the selected url
			new URL(url);
			return true;
		} catch (MalformedURLException e) {
			// The selected url was not valid
			e.printStackTrace();
			return false;
		}
	}
	
	// Response handler
	ResponseHandler<String> responseHandler = new ResponseHandler<String>() {
		// invoked when client receives response
		public String handleResponse(HttpResponse response)
				throws ClientProtocolException, IOException  {
			// get response entity
			HttpEntity httpEntity = response.getEntity();
			// read the response as byte array
			StringBuffer responseBuffer = new StringBuffer();
			byte[] responseBytes = EntityUtils.toByteArray(httpEntity);
			// write the response byte array to a string buffer
			responseBuffer.append(new String(responseBytes, 0, responseBytes.length));       
			return responseBuffer.toString();
		}
	};
}