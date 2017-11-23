#import "APIBaseHandler.h"

@interface APIRetrieveInfectionPressureHandler : APIBaseHandler

- (void)retrieveInfectionPressure:(NSString *)username
                         password:(NSString *)password
                     locationName:(NSString *)locationName
                     locationCoordinate:(NSString *)locationCoordinate
                            width:(NSInteger)width
                           height:(NSInteger)height
                   successHandler:(SuccessBlock)successHandler
                     errorHandler:(ErrorBlock)errorHandler;

@end
