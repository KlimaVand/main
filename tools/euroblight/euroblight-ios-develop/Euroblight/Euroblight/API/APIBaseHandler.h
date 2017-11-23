#import <Foundation/Foundation.h>

@interface APIBaseHandler : NSObject <NSXMLParserDelegate>

extern NSString *const SOAP_HEADER_BASE;
extern NSString *const SOAP_ENVELOPE_START;
extern NSString *const SOAP_BODY_START;
extern NSString *const SOAP_BODY_ACTION_START;
extern NSString *const SOAP_BODY_PARAMETER;
extern NSString *const SOAP_BODY_ACTION_END;
extern NSString *const SOAP_BODY_END;
extern NSString *const SOAP_ENVELOPE_END;
extern NSString *const SOAP_ACTION_BASE;

// API
extern NSString *const SOAP_ACTION_UPLOAD_OBSERVATION;
extern NSString *const SOAP_ACTION_MAP_KML_FILE;
extern NSString *const SOAP_ACTION_INFECTION_PRESSURE;
extern NSString *const SOAP_SERVICE_URL;
extern NSString *const KEY_LOGIN;
extern NSString *const KEY_PASSWORD;
extern NSString *const KEY_COUNTRY_ID;
extern NSString *const KEY_LOCATION_NAME;
extern NSString *const KEY_LOCATION_COORDINATE;
extern NSString *const KEY_WIDTH;
extern NSString *const KEY_HEIGHT;
extern NSString *const KEY_LATITUDE;
extern NSString *const KEY_LONGITUDE;
extern NSString *const KEY_SURVEY_SITE_ID;
extern NSString *const KEY_SURVEY_NAME;
extern NSString *const KEY_INSTITUTION_NAME;
extern NSString *const KEY_SEVERITY_ID;
extern NSString *const KEY_GROWTH_STAGE_ID;
extern NSString *const KEY_IMAGE;
extern NSString *const KEY_FIELD_NAME;
extern NSString *const KEY_VARIETY;
extern NSString *const KEY_OBSERVATION_DATE;

//RESPONE CODES
extern NSInteger const RESPONSE_SUCCES;

// ERRORS
extern NSInteger const ERROR_PARSE_CODE;
extern NSInteger const ERROR_COMMUNICATION_CODE;
extern NSInteger const ERROR_UNAUTHORIZED;
extern NSInteger const ERROR_SERVER_FAILED;

// HANDLERS
typedef void (^SuccessBlock)(NSString *result);
typedef void (^ErrorBlock)(NSNumber *errorCode);

@property (strong, nonatomic) NSURLSessionDataTask *dataTask;
@property (strong, atomic) NSString *responseResult;
@property (nonatomic, strong) NSDateFormatter *dateFormatter;

- (NSMutableURLRequest *)buildURLRequest;

- (void)configureURLRequest:(NSMutableURLRequest **)request
              forSoapAction:(NSString *)soapAction
          withContentLength:(NSInteger)contentLength;

- (BOOL)validateUrl: (NSString *)candidate;
@end
