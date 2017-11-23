#import <Foundation/Foundation.h>

@interface PickerValue : NSObject <NSCoding>

@property (strong, nonatomic) NSString *name;
@property (strong, nonatomic) NSString *value;
@property (nonatomic) NSNumber *id;
@property (nonatomic) NSInteger index;

- (id)initWithJSONDictionary:(NSDictionary *)dictionary index:(NSInteger)index;

@end
