#import "APIUploadReportHandler.h"
#import "Report.h"

@implementation APIUploadReportHandler

#pragma mark - Public Methods

- (id)initWithReport:(Report *)report {
    self = [super init];
    if(self) {
        self.report = report;
    }
    return self;
}

- (void)uploadReportWithUsername:(NSString *)username
                        password:(NSString*)password
                  successHandler:(SuccessReportBlock)successHandler
                    errorHandler:(ErrorReportBlock)errorHandler {
    NSData *imageData = UIImagePNGRepresentation(self.report.photo);
    
    // Build the soap message
    NSString *soapMessage = [NSString stringWithFormat:@"%@%@%@%@%@%@%@%@%@%@%@%@%@%@%@%@%@%@%@%@%@",
                             SOAP_HEADER_BASE,
                             SOAP_ENVELOPE_START,
                             SOAP_BODY_START,
                             [NSString stringWithFormat:SOAP_BODY_ACTION_START,SOAP_ACTION_UPLOAD_OBSERVATION],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LOGIN,username,KEY_LOGIN],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_PASSWORD,password,KEY_PASSWORD],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LATITUDE,self.report.latitudeValue ,KEY_LATITUDE],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LONGITUDE,self.report.longitudeValue,KEY_LONGITUDE],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LOCATION_NAME,self.report.locationName,KEY_LOCATION_NAME],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_SURVEY_SITE_ID,self.report.typeOfProduction.id.stringValue,KEY_SURVEY_SITE_ID],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_SURVEY_NAME,self.report.surveyorName,KEY_SURVEY_NAME],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_INSTITUTION_NAME,self.report.institutionName,KEY_INSTITUTION_NAME],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_GROWTH_STAGE_ID,self.report.BBCH.id.stringValue,KEY_GROWTH_STAGE_ID],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_SEVERITY_ID,self.report.severity.id.stringValue,KEY_SEVERITY_ID],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_IMAGE,(imageData != nil) ? [imageData base64EncodedStringWithOptions:0] : @"",KEY_IMAGE],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_FIELD_NAME,self.report.fieldName,KEY_FIELD_NAME],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_VARIETY,self.report.variety,KEY_VARIETY],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_OBSERVATION_DATE,[self.dateFormatter stringFromDate:self.report.observationDate],KEY_OBSERVATION_DATE],
                             [NSString stringWithFormat:SOAP_BODY_ACTION_END,SOAP_ACTION_UPLOAD_OBSERVATION],
                             SOAP_BODY_END,
                             SOAP_ENVELOPE_END
                             ];
    NSMutableURLRequest *request = [self buildURLRequest];
    [self configureURLRequest:&request forSoapAction:SOAP_ACTION_UPLOAD_OBSERVATION withContentLength:[soapMessage length]];
    [request setHTTPMethod:@"POST"];
    [request setHTTPBody: [soapMessage dataUsingEncoding:NSUTF8StringEncoding]];
    
    self.dataTask = [[NSURLSession sharedSession] dataTaskWithRequest:request
        completionHandler:^(NSData *data,
        NSURLResponse *response,
        NSError *error) {
          [self responseHandlerWithData:data
                               response:response
                                error:error
                        successHandler:successHandler
                          errorHandler:errorHandler];
    }];
    [self.dataTask resume];
}

#pragma mark - Private Methods

- (void)responseHandlerWithData:(NSData *)data
                       response:(NSURLResponse *)response
                          error:(NSError*)error
                 successHandler:(SuccessReportBlock)successHandler
                   errorHandler:(ErrorReportBlock)errorHandler {
    if (response == nil || error != nil) {
        errorHandler(@(ERROR_COMMUNICATION_CODE), self.report);
        return;
    }
    NSXMLParser *xmlParser = [[NSXMLParser alloc] initWithData:data];
    xmlParser.delegate = self;
    // Run the parser
    if (![xmlParser parse]) {
        errorHandler(@(ERROR_PARSE_CODE), self.report);
        return;
    }
    if ([self.responseResult intValue] == RESPONSE_SUCCES) {
        successHandler(self.responseResult, self.report);
        return;
    }
    if([self.responseResult intValue] == ERROR_UNAUTHORIZED) {
        errorHandler(@(ERROR_UNAUTHORIZED), self.report);
        return;
    }
    //Unkown errors treated as ERROR_SERVER_FAILED
    errorHandler(@(ERROR_SERVER_FAILED), self.report);
}

@end
