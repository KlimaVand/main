#import <Foundation/Foundation.h>
#import "PickerValue.h"

@interface Report : NSObject <NSCoding>

@property (strong, nonatomic) PickerValue *BBCH;
@property (strong, nonatomic) NSString *institutionName;
@property (strong, nonatomic) NSString *latitudeValue;
@property (strong, nonatomic) NSString *longitudeValue;
@property (strong, nonatomic) NSString *locationName;
@property (strong, nonatomic) NSString *fieldName;
@property (strong, nonatomic) NSDate *observationDate;
@property (strong, nonatomic) UIImage *photo;
@property (strong, nonatomic) PickerValue *typeOfProduction;
@property (strong, nonatomic) PickerValue *severity;
@property (strong, nonatomic) NSString *surveyorName;
@property (strong, nonatomic) NSString *variety;

@end
