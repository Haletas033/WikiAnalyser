# WikiAnalyser

## About:
WikiAnalyser is a lightweight GUI Application built using only C90, Win32, Curl, and the C standard library.
WikiAnalyser is an all in solution to download wikipedia dumps, clean them up, create an article struct, parse, and graph.
The main exe is less than 400kb though it does auto-download Zig ~160mb, TopN file ~700mb and a Wikipedia dump up to 100gb

## Current Features:
### Cleanup Features:
* Set what containers, strings, and characters you want removed
* Single-pass efficient clean up algorithm for large files

### Parsing Features:
* Create an article struct containing the fields you would like to analyse
* Write zig that runs on each character or multiple (experimental / Requires user to ensure safety)

### Analysis Feature:
* Multiple graph types: Pie, Percent Bar, Bar, and Scatter
* Analyse by article name, average, or sum (sum and average will look identical on current graphs)

### Known Issues:
* Console shows nothing (the app opens an instance of cmd which you can use for now)
* With many fields some may go of screen (no current fix will work when scrollbars are added)
* Dropdowns sometimes clip into each other on small resolutions
* Download Zig currently fails to unzip sometimes (please manually download for now)

### Upcoming Features:
* Console and scrollbar support
* OnAnalyse function for advanced analysis
* More graph types
* Multi-article graphing
* Faster functions
* Finish LINUXGUI

### Getting Started:
* Download latest release
* extract zip to your desired location
* Run WikiAnalyser.exe 
#### Note: Only Windows is supported at the moment

### Simple Example (Count A's vs E's):
```zig
    const std = @import("std");
    const Io = std.Io;
    
    const root = @import("root.zig");
    
    //Runs on every article
    pub export fn OnArticle(currentChar: [*]c_char, article: *root.Article) callconv(.c) void {
        const char: [*]u8 = @ptrCast(currentChar);
        if (char[0] == 'a'){
            (root.Field(c_int, "A Count", article) orelse return).*+=1;
        }
        else if (char[0] == 'e'){
            (root.Field(c_int, "E Count", article) orelse return).*+=1;
        }
}
```
#### Note: In Fields you have to add two intFields 'A Count' and 'E Count'

### Contributions / Feedback
All bug reports, contribution, and feedback are supported. Just open an issue and I should get back to you.

## License

This project is licensed under the [MIT License](LICENSE).

