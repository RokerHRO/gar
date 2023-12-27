# <#gar#> — The Git Archiver

Git (and other version control systems) doesn't handle binary files very well. In contrast to plaintext files (e.g. source code) it is hard or even impossible to do a proper diff or merge on binary files.

The Git Archiver is an approach to make it better. It converts binary files into a format that is handled as "text files" by git so it offer the posibility to diff & merge the files and for well-known binary formats it offers a semantic sensible human-readable visualization of the content and of the diffs.

First goal is to enable this for XML-based office documents (Open Document and Microsoft's "OOXML" format), but other binary files, e.g. bitmap images, shall be possible, too.

## Usage

### Create an archive
* `gar example.odt` — creates a text file `example.odt.gar` which can be added to yout git/mercurial repository

### Recover the original file
* `ungar example.odt.gar` or `gar -d example.odt.gar` — creates the original file (or at least a semantically equivalent file)

## Design goals
* Plaintext format. File content is UTF-8, control structures are ASCII.
* Human-readable content (everything that is some kind of "text" in a broader sense) shall be human-readable in the archive, too.
* Non-textual (e.g. images, audio) content might be presented in a text-alike format, if possible
* extensible encoder/decoder ("codec") for different file types / content types
  * human-readable meta data 
  * compressed files are stored uncompressed
  * container files (storing several individual files) are stored file-by-file
  * "minified" XML and JSON files are stored beautified and well-indented
  * binary files that cannot be converted sensibly in a text form are stored in Siso93 format, that allows chunking
  * bitmap images can contain an ASCII "thumbnail" to give a visual hint about changes in the image (optional)

## File format description

Every gar file starts with a common header that marks it as gar file, contains a version number and the "codec" that was used to encode the original file content. Some codecs (e.g. codecs for archive file formats) split the gar file in independent "chapters" (e.g. one for each archived file) that might be encoded by their own codecs, whatever might be most appropriate for every chapter.

Every codec ensures, that no file content can be encoded into output that can be interpreted as gar header, footer or chapter header lines.

### Header

The header consists of one line, terminated by newline:

> `<#gar#2.0#` _codec_ [`#` _parameters_ ] `#>`

* The spaces shown in the header line are just for illustration.
* The parameters are optional and codec-specific.
* The "newline" might be a single NL character or a CR-NL sequence.

### Footer

Every gar file ends with a footer line, terminated also by a newline:

> `<#endgar` [`#sha256=` _sha256-sum_ ] `#>` 

* The spaces shown in the footer line are just for illustration.
* The SHA-256 sum is optional. It is computed on the original (unencoded) file content and might either be 64 hex digits (0-9a-f), or truncated to the first 16 or 32 hex digits.
* The "newline" might be a single NL character or a CR-NL sequence.

### Chapter header line

A chapter header consists of one line, terminated by newline:

> `<|` _codec_ [ `|` _parameters_ ] [ `|"` _chapter_name_ `"` ] `|>`

* The spaces shown in the footer line are just for illustration.
* The parameters are optional and a comma-separated key-value list. Their meaning is codec-specific.
* The chapter name is optional and a JSON string in double-quotes.
* The "newline" might be a single NL character or a CR-NL sequence.
* There is no "chunk footer".

### Codecs

#### plain – plaintext
* This is used for every data that is considered as "plaintext", e.g. source code
* If the data contain lines that parses as gar header or footer lines these lines are escaped in a special way:
  `~(` _line_ `)~`
  Optionally overlong lines can be escaped the same way and folded at a suitable position.

#### siso93 – tagged encoding for arbitrary binary data
* All data that is not "plaintext" – and doesn't have a format-specific codec – is considered as "binary" data and encoded with this codec.
* Unlike base64 or base85 this encoding keeps ASCII text snippets in the data human-readable. To make it diff-friendly (a local change, insertion od deletion of data results in a small diff) a special encoding is used:
  1. A rolling checksum is used to split the data in content-dependent chunks. The minimum & maximum lenghts of the chunks can be configured.
  2. Within each chunk every 4 (binary data) octets are encoded into 1 "tag" char and 4 "data" chars. These chars are only printable ASCII chars.
  3. Each chunk is encoded in one "tag line", followed by one "data line".

#### xmltidy - pretty-printed XML
* XML data that is stored "pretty-printed" with proper line breaks and indentation, making it diff-friendly
* Because the original layout of the XML data is lost, **this codec is not reversible!**

#### jsontidy – pretty-printed JSON
* JSON data that is stored "pretty-printed" with proper line breaks and indentation, making it diff-friendly
* Because the original layout of the JSON data is lost, **this codec is not reversible!**

#### zip – for ZIP files, JAR archives, office files, too (Open Document and Microsoft's OOXML)
* The files in the ZIP file are stored as "chapters" in the gar file, in the same order as in he original ZIP file.
* Same data can be compressed differently by different ZIP encoders and not all ZIP metadata are represented exactly in gar, so **this codec is no reversible!**
* Nested ZIP files (ZIP file containing another ZIP file) shall be stored in a "flattened" way to avoid double-encoding of the content of the inner ZIP file. The files are linked to their ZIP container via "id" and "parent":

``` 
    outer.zip
    ├ readme.txt
    ├ inner.zip
    │ ├ foo.txt 
    │ └ bar.txt
    └ other.txt
```
It would look as gar file this way:

```
<#gar#2.0#zip#version_made_by=0x33F#>
<|plain|charset=utf8|"readme.txt"|>
This is the content of the readme.txt file.
Any gar-specific markup must be escaped here.
<|zip|version_mady_by=0x33F|id=123|"inner.zip"|>
<|plain|charset=utf8|parent=123|"foo.txt"|>
This is content of foo.txt
<|plain|charset=utf8|parent=123|"bar.txt"|>
This is content of bar.txt
<|plain|charset=utf8|"other.txt"|>
This is the content of other.txt, _not_ part of the inner.zip!
<#endgar#>
```
