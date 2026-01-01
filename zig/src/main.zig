const std = @import("std");
const Io = std.Io;

pub export fn zigPrintTest(text: [*:0]const u8) callconv(.c) void{
    std.debug.print("{s}\n", .{text});
}