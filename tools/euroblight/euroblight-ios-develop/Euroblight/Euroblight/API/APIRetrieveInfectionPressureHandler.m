#import "APIRetrieveInfectionPressureHandler.h"

@implementation APIRetrieveInfectionPressureHandler

- (void)retrieveInfectionPressure:(NSString *)username
                         password:(NSString *)password
                     locationName:(NSString *)locationName
                     locationCoordinate:(NSString *)locationCoordinate
                            width:(NSInteger)width
                           height:(NSInteger)height
                   successHandler:(SuccessBlock)successHandler
                     errorHandler:(ErrorBlock)errorHandler {
    // Build the soap message
    NSString *soapMessage = [NSString stringWithFormat:@"%@%@%@%@%@%@%@%@%@%@%@%@%@",
                             SOAP_HEADER_BASE,
                             SOAP_ENVELOPE_START,
                             SOAP_BODY_START,
                             [NSString stringWithFormat:SOAP_BODY_ACTION_START,SOAP_ACTION_INFECTION_PRESSURE],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LOGIN,username,KEY_LOGIN],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_PASSWORD,password,KEY_PASSWORD],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LOCATION_NAME,locationName,KEY_LOCATION_NAME],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LOCATION_COORDINATE,locationCoordinate,KEY_LOCATION_COORDINATE],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_WIDTH,[NSString stringWithFormat:@"%ld",(long)width],KEY_WIDTH],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_HEIGHT,[NSString stringWithFormat:@"%ld",(long)height],KEY_HEIGHT],
                             [NSString stringWithFormat:SOAP_BODY_ACTION_END,SOAP_ACTION_INFECTION_PRESSURE],
                             SOAP_BODY_END,
                             SOAP_ENVELOPE_END
                             ];
    
    NSMutableURLRequest *request = [self buildURLRequest];
    [self configureURLRequest:&request forSoapAction:SOAP_ACTION_INFECTION_PRESSURE withContentLength:[soapMessage length]];
    [request setHTTPMethod:@"POST"];
    [request setHTTPBody: [soapMessage dataUsingEncoding:NSUTF8StringEncoding]];
    
    self.dataTask = [[NSURLSession sharedSession] dataTaskWithRequest:request
        completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {
            [self responseHandlerWithData:data
                                 response:response
                                    error:error
                           successHandler:successHandler
                             errorHandler:errorHandler];
        }];
    [self.dataTask resume];
}

- (void)responseHandlerWithData:(NSData *)data
                       response:(NSURLResponse *)response
                          error:(NSError*)error
                 successHandler:(SuccessBlock)successHandler
                   errorHandler:(ErrorBlock)errorHandler {
    if (response == nil || error != nil) {
        errorHandler(@(ERROR_COMMUNICATION_CODE));
        return;
    }
    NSXMLParser *xmlParser = [[NSXMLParser alloc] initWithData:data];
    xmlParser.delegate = self;
    // Run the parser
    if (![xmlParser parse]) {
        errorHandler(@(ERROR_PARSE_CODE));
        return;
    }
    NSLog(@"Soap response: %@",self.responseResult);
    if ([self validateUrl:self.responseResult]) {
        // Validate i response is an URL
        successHandler(self.responseResult);
        return;
    }
    if ([self.responseResult intValue] == ERROR_UNAUTHORIZED) {
        errorHandler(@(ERROR_UNAUTHORIZED));
        return;
    }
    //Unkown errors treated as ERROR_SERVER_FAILED
    errorHandler(@(ERROR_SERVER_FAILED));
}


@end
