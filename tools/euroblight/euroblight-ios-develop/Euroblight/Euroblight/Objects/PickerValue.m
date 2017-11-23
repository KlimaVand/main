#import "PickerValue.h"

NSString *const NAME_KEY = @"NameKey";
NSString *const VALUE_KEY = @"ValueKey";
NSString *const ID_KEY = @"IdKey";
NSString *const INDEX_KEY = @"IndexKey";

@implementation PickerValue

- (id)initWithJSONDictionary:(NSDictionary *)dictionary index:(NSInteger)index {
    self = [super init];
    if(self) {
        self.name = [dictionary objectForKey:@"name"];
        self.value = [dictionary objectForKey:@"value"];
        self.id = [dictionary objectForKey:@"id"];
        self.index = index;
    }
    return self;
}

- (void) encodeWithCoder:(NSCoder *)encoder {
    [encoder encodeObject:self.name forKey:NAME_KEY];
    [encoder encodeObject:self.value forKey:VALUE_KEY];
    [encoder encodeObject:self.id forKey:ID_KEY];
    [encoder encodeInteger:self.index forKey:INDEX_KEY];
}

- (id)initWithCoder:(NSCoder *)decoder {
    self.name = [decoder decodeObjectForKey:NAME_KEY];
    self.value = [decoder decodeObjectForKey:VALUE_KEY];
    self.id = [decoder decodeObjectForKey:ID_KEY];
    self.index = [decoder decodeIntegerForKey:INDEX_KEY];
    return self;
}

@end
