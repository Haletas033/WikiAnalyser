const std = @import("std");
const Io = std.Io;

pub const Article = extern struct {
    title: [*]c_char,

    intFields: [*]c_int,
    floatFields: [*]f32,
    boolFields: [*]u8,
    stringFields: [*][*]c_char,

    intFieldsSize: c_int,
    floatFieldsSize: c_int,
    boolFieldsSize: c_int,
    stringFieldsSize: c_int,

    intFieldNames: [*][*]c_char,
    floatFieldNames: [*][*]c_char,
    boolFieldNames: [*][*]c_char,
    stringFieldNames: [*][*]c_char,
};

//Convert [*]c_char to [*:0]const u8
pub fn toString(text: ?[*]c_char) [*:0]const u8 {
    if (text != null) return @ptrCast(text);
    return "";
}

pub fn Title(article: *Article) [*]c_char{
    return article.title;
}

//Get a field by name and type
pub fn Field(comptime T: type, name: [*:0]const u8, article: *Article) *T {

    var names: [*][*]c_char = undefined;
    var size: c_int = undefined;
    var fields: [*]T = undefined;

    if (std.meta.eql(T, c_int)) {
        names = article.intFieldNames;
        size = article.intFieldsSize;
        fields = @alignCast(@ptrCast(article.intFields));
    } else if (std.meta.eql(T, f32)){
        names = article.floatFieldNames;
        size = article.floatFieldsSize;
        fields = @alignCast(@ptrCast(article.floatFields));
    } else if (std.meta.eql(T, u8)){
        names = article.boolFieldNames;
        size = article.boolFieldsSize;
        fields = @alignCast(@ptrCast(article.boolFields));
    } else if (std.meta.eql(T, [*]c_char)){
        names = article.stringFieldNames;
        size = article.stringFieldsSize;
        fields = @alignCast(@ptrCast(article.stringFields));
    } else {
        @panic("Unsupported type. Supported types are: c_int, f32, u8, [*]c_char");
    }

    const nameSlice: []const u8 = std.mem.span(name);

    for (0..@intCast(size)) |i|{
        const currentSlice: []const u8 = std.mem.span(toString(names[i]));
        if (std.mem.eql(u8, currentSlice, nameSlice)) return @ptrCast(&fields[i]);
    }

    @panic("Couldn't find field");
}
