const std = @import("std");
const Io = std.Io;

const root = @import("root.zig");

//Runs on every article
pub export fn OnArticle(currentChar: [*]c_char, article: *root.Article) callconv(.c) void {
    const char: [*]u8 = @ptrCast(currentChar);
    if (char[0] == 'e'){
        root.Field(c_int, "e_Count", article).*+=1;
    }
}

pub export fn zigPrintTest(article: *root.Article) callconv(.c) void {
    std.debug.print("Title: {s}\n", .{root.toString(root.Title(article))});
    std.debug.print("e_Count: {}\n", .{root.Field(c_int, "e_Count", article).*});
    std.debug.print("Float Test: {}\n", .{root.Field(f32, "Float Test", article).*});
    std.debug.print("Bool Test: {}\n", .{root.Field(u8, "Bool Test", article).*});
    std.debug.print("String: {s}\n\n", .{root.toString(root.Field([*]c_char, "String Test", article).*)});
}