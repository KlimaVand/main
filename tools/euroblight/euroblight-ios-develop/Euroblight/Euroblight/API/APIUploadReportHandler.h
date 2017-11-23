#import <Foundation/Foundation.h>
#import "APIBaseHandler.h"
@class Report;


@interface APIUploadReportHandler : APIBaseHandler

typedef void (^SuccessReportBlock)(NSString *result, Report *report);
typedef void (^ErrorReportBlock)(NSNumber *errorCode, Report *report);

@property (strong, nonatomic) Report *report;

- (id)initWithReport:(Report *)report;

- (void)uploadReportWithUsername:(NSString *)username
                        password:(NSString*)password
                  successHandler:(SuccessReportBlock)successHandler
                    errorHandler:(ErrorReportBlock)errorHandler;

@end
