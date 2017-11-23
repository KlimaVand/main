#import <Foundation/Foundation.h>

@interface PickerValueManager : NSObject

+ (PickerValueManager *)sharedManager;

@property (strong, nonatomic) NSArray *bbchValues;
@property (strong, nonatomic) NSArray *severityValues;
@property (strong, nonatomic) NSArray *typeOfProductionValues;
@property (strong, nonatomic) NSArray *infectionPressureCountries;
@property (strong, nonatomic) NSArray *infectionPressureLocations;

@end
