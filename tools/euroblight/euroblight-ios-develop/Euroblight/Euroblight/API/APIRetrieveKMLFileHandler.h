#import <Foundation/Foundation.h>
#import "APIBaseHandler.h"

@interface APIRetrieveKMLFileHandler : APIBaseHandler

- (void)retrieveKMLFileForUsername:(NSString *)username
               password:(NSString *)password
              countryId:(NSString *)countryId
         successHandler:(SuccessBlock)successHandler
           errorHandler:(ErrorBlock)errorHandler;

@end
