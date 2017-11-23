package com.af.euroblight2.handlers;

public interface CommunicationManagerHandler {
	
	public enum ERRORS_DEFINITION_CODE {
		COMMUNICATION_ERROR(-1), 
		PARSE_ERROR(-2),
		ERROR_SERVER_FAILED(50),
		ERROR_UNAUTHORIZED(41);

	    ERRORS_DEFINITION_CODE (int i) {
	        this.type = i;
	    }

	    private int type;

	    public int getNumericType() {
	        return type;
	    }
	    
	    public static ERRORS_DEFINITION_CODE getCode(int code) {
	    	switch(code) {
	    	case -1:
	    		return COMMUNICATION_ERROR;
	    	case -2:
	    		return PARSE_ERROR;
	    	case 50:
	    		return ERROR_SERVER_FAILED;
	    	case 41:
	    		return ERROR_UNAUTHORIZED;
	    	default:
	    		return null;
	    	} 
	    }
	}
												 
	void successHandler(String result);
	void errorHandler(ERRORS_DEFINITION_CODE errorCode);
}
