# <#gar#> — The Git Archiver

Git and other version control systems handle binary files not very well. In contrast to plaintext files (e.g. source code) it is hard or even impossible to do a proper diff or merge on binary files.

The Git Archiver is an approach to make it better. It converts binary files into a format that is handled as "text files" by git so it offer the posibility to diff & merge the files and for well-known binary formats it offers a semantic sensible human-readable visualization of the content and of the diffs.

First goal is to enable this for XML-based office documents (Open Document and Microsoft's "OOXML" format), but other binary files, e.g. bitmap images, shall be possible, too.

## Usage

### Create an archive
* `gar example.odt` — creates a text file `example.odt.gar` which can be added to yout git/mercurial repository

### Recover the original file
* `ungar example.odt.gar` or `gar -d example.odt.gar` — creates the original file (or at least a semantically equivalent file)
