#import "APIRetrieveKMLFileHandler.h"

@implementation APIRetrieveKMLFileHandler

- (void)retrieveKMLFileForUsername:(NSString *)username
               password:(NSString *)password
              countryId:(NSString *)countryId
         successHandler:(SuccessBlock)successHandler
           errorHandler:(ErrorBlock)errorHandler {
    // Build the soap message
    NSString *soapMessage = [NSString stringWithFormat:@"%@%@%@%@%@%@%@%@%@%@",
                             SOAP_HEADER_BASE,
                             SOAP_ENVELOPE_START,
                             SOAP_BODY_START,
                             [NSString stringWithFormat:SOAP_BODY_ACTION_START,SOAP_ACTION_MAP_KML_FILE],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_LOGIN,username,KEY_LOGIN],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_PASSWORD,password,KEY_PASSWORD],
                             [NSString stringWithFormat:SOAP_BODY_PARAMETER,KEY_COUNTRY_ID,countryId,KEY_COUNTRY_ID],
                             [NSString stringWithFormat:SOAP_BODY_ACTION_END,SOAP_ACTION_MAP_KML_FILE],
                             SOAP_BODY_END,
                             SOAP_ENVELOPE_END
                             ];
    
    NSMutableURLRequest *request = [self buildURLRequest];
    [self configureURLRequest:&request forSoapAction:SOAP_ACTION_MAP_KML_FILE withContentLength:[soapMessage length]];
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

#pragma mark - Private Methods

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
