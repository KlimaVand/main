#import "APIBaseHandler.h"

@implementation APIBaseHandler

// Soap base
NSString *const SOAP_HEADER_BASE = @"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
NSString *const SOAP_ENVELOPE_START = @"<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:ns1=\"http://tempuri.org/\">\n";
NSString *const SOAP_BODY_START = @"<SOAP-ENV:Body>\n";
NSString *const SOAP_BODY_ACTION_START = @"<ns1:%@>\n";
NSString *const SOAP_BODY_PARAMETER = @"<ns1:%@>%@</ns1:%@>\n";
NSString *const SOAP_BODY_ACTION_END = @"</ns1:%@>\n";
NSString *const SOAP_BODY_END = @"</SOAP-ENV:Body>\n";
NSString *const SOAP_ENVELOPE_END = @"</SOAP-ENV:Envelope>";
NSString *const SOAP_ACTION_BASE = @"http://tempuri.org/IServiceSurvey/";

// API
NSString *const SOAP_ACTION_UPLOAD_OBSERVATION = @"UploadObservation";
NSString *const SOAP_ACTION_MAP_KML_FILE = @"MapKMLFile";
NSString *const SOAP_ACTION_INFECTION_PRESSURE = @"InfectionPressure";
NSString *const SOAP_SERVICE_URL = @"http://130.226.173.136/ProjectNET/Service/SurveyPLB/ServiceSurvey.svc";
NSString *const KEY_LOGIN = @"aLoginName";
NSString *const KEY_PASSWORD = @"aPassWord";
NSString *const KEY_COUNTRY_ID = @"aCountryID";
NSString *const KEY_LOCATION_NAME = @"aLocationName";
NSString *const KEY_LOCATION_COORDINATE = @"aLocationCoordinate";
NSString *const KEY_WIDTH = @"aWidth";
NSString *const KEY_HEIGHT = @"aHeight";
NSString *const KEY_LATITUDE = @"aLatitude";
NSString *const KEY_LONGITUDE = @"aLongitude";
NSString *const KEY_SURVEY_SITE_ID = @"aSurveySiteID";
NSString *const KEY_SURVEY_NAME = @"aSurveyorName";
NSString *const KEY_INSTITUTION_NAME = @"aInstitutionName";
NSString *const KEY_SEVERITY_ID = @"aSeverityID";
NSString *const KEY_GROWTH_STAGE_ID = @"aGrowthStageID";
NSString *const KEY_IMAGE = @"aImage";
NSString *const KEY_FIELD_NAME = @"aFieldName";
NSString *const KEY_VARIETY = @"aVariety";
NSString *const KEY_OBSERVATION_DATE = @"aObservationDate";

// RESPONSE CODES
NSInteger const RESPONSE_SUCCES = 20;

// ERRORS
NSInteger const ERROR_COMMUNICATION_CODE = -1;
NSInteger const ERROR_PARSE_CODE = -2;
NSInteger const ERROR_UNAUTHORIZED = 41;
NSInteger const ERROR_SERVER_FAILED = 50;

- (id)init {
    self = [super init];
    if(self) {
        self.dateFormatter = [[NSDateFormatter alloc] init];
        [self.dateFormatter setDateFormat:@"MM-dd-yyyy"];
    }
    return self;
}

- (NSMutableURLRequest *)buildURLRequest  {
    NSURL *url = [NSURL URLWithString:SOAP_SERVICE_URL];
    return [NSMutableURLRequest requestWithURL:url];
}

- (void)configureURLRequest:(NSMutableURLRequest **)request
              forSoapAction:(NSString *)soapAction
          withContentLength:(NSInteger)contentLength {
    [*request addValue: @"text/xml; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    [*request addValue:[NSString stringWithFormat:@"%@%@", SOAP_ACTION_BASE, soapAction] forHTTPHeaderField:@"soapaction"];
    [*request addValue: [NSString stringWithFormat:@"%ld", (long)contentLength] forHTTPHeaderField:@"Content-Length"];
}

- (BOOL)validateUrl: (NSString *)candidate {
   NSString *urlRegExp = @"(?i)\\b(?:[a-z][\\w-]+:(?:/{1,3}|[a-z0-9%])|www\\d{0,3}[.]|[a-z0-9.\\-]+[.][a-z]{2,4}/)(?:[^\\s()<>]+|\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\))+(?:\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\)|[^\\s`!()\\[\\]{};:'\".,<>?«»“”‘’])";
    NSPredicate *urlTest = [NSPredicate predicateWithFormat:@"SELF MATCHES %@", urlRegExp];
    return [urlTest evaluateWithObject:candidate];
}

#pragma mark NSXMLParserDelegate

- (void)parser:(NSXMLParser *)parser foundCharacters:(NSString *)string {
    self.responseResult = string;
}

@end
