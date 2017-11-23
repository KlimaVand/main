package com.af.euroblight.handlers;

import com.af.euroblight.objects.Report;

public interface ReportCommunicationManagerHandler extends CommunicationManagerHandler {
	
	void successHandler(Report report, String result);
	void errorHandler(Report report, ERRORS_DEFINITION_CODE errorCode);
}
