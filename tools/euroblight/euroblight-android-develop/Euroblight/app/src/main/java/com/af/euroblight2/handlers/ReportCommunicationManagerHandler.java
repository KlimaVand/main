package com.af.euroblight2.handlers;

import com.af.euroblight2.objects.Report;

public interface ReportCommunicationManagerHandler extends CommunicationManagerHandler {
	
	void successHandler(Report report, String result);
	void errorHandler(Report report, ERRORS_DEFINITION_CODE errorCode);
}
