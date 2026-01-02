const std = @import("std");
const Io = std.Io;

const root = @import("root.zig");

pub export fn zigPrintTest(article: *root.Article) callconv(.c) void {
    std.debug.print("{s}: ", .{root.toString(article.intFieldNames[0])});
    std.debug.print("{d}\n", .{article.intFields[0]});

    std.debug.print("{s}: ", .{root.toString(article.floatFieldNames[0])});
    std.debug.print("{d}\n", .{article.floatFields[0]});

    std.debug.print("{s}: ", .{root.toString(article.boolFieldNames[0])});
    std.debug.print("{d}\n", .{article.boolFields[0]});

    std.debug.print("{s}: ", .{root.toString(article.stringFieldNames[0])});
    std.debug.print("\"{s}\"\n\n", .{root.toString(article.stringFields[0])});

    root.Field(c_int, "Int Test", article).* = 42;
    root.Field(f32, "Float Test", article).* = 2.71828;
    root.Field(u8, "Bool Test", article).* = @intFromBool(true);
    root.Field([*]c_char, "String Test", article).* = @constCast(@ptrCast("67"));

    std.debug.print("Int Test: {}\n", .{root.Field(c_int, "Int Test", article).*});
    std.debug.print("Float Test: {}\n", .{root.Field(f32, "Float Test", article).*});
    std.debug.print("Bool Test: {}\n", .{root.Field(u8, "Bool Test", article).*});
    std.debug.print("String: {s}\n", .{root.toString(root.Field([*]c_char, "String Test", article).*)});
}