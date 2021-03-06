[![Daring Fireball](https://daringfireball.net/graphics/logos/)](https://daringfireball.net/ "Daring Fireball: Home")

By **JOHN GRUBER**

-   [ARCHIVE](https://daringfireball.net/archive/ "Previous articles.")

-   [THE TALK SHOW](https://daringfireball.net/thetalkshow/ "America’s favorite 3-star podcast.")
-   [DITHERING](https://dithering.fm/ "Three episodes per week, 15 minutes per episode. Not a minute less, not a minute more.")
-   [PROJECTS](https://daringfireball.net/projects/ "Software projects, including SmartyPants and Markdown.")
-   [CONTACT](https://daringfireball.net/contact/ "How to send email or feedback regarding Daring Fireball.")
-   [COLOPHON](https://daringfireball.net/colophon/ "About this site and the tools used to produce it.")
-   [RSS FEED](https://daringfireball.net/feeds/)
-   [TWITTER](https://twitter.com/daringfireball)
-   [SPONSORSHIP](https://daringfireball.net/feeds/sponsors/)

# Markdown

-   Main
-   [Basics](https://daringfireball.net/projects/markdown/basics "Markdown Basics")
-   [Syntax](https://daringfireball.net/projects/markdown/syntax "Markdown Syntax Documentation")
-   [License](https://daringfireball.net/projects/markdown/license "Pricing and License Information")
-   [Dingus](https://daringfireball.net/projects/markdown/dingus "Online Markdown Web Form")

## DOWNLOAD

[Markdown 1.0.1](https://daringfireball.net/projects/downloads/Markdown_1.0.1.zip) (18 KB) — 17 Dec 2004

## INTRODUCTION

Markdown is a text-to-HTML conversion tool for web writers. Markdown allows you to write using an easy-to-read, easy-to-write plain text format, then convert it to structurally valid XHTML (or HTML).

Thus, “Markdown” is two things: (1) a plain text formatting syntax; and (2) a software tool, written in Perl, that converts the plain text formatting to HTML. See the [Syntax](https://daringfireball.net/projects/markdown/syntax) page for details pertaining to Markdown’s formatting syntax. You can try it out, right now, using the online [Dingus](https://daringfireball.net/projects/markdown/dingus).

The overriding design goal for Markdown’s formatting syntax is to make it as readable as possible. The idea is that a Markdown-formatted document should be publishable as-is, as plain text, without looking like it’s been marked up with tags or formatting instructions. While Markdown’s syntax has been influenced by several existing text-to-HTML filters, the single biggest source of inspiration for Markdown’s syntax is the format of plain text email.

The best way to get a feel for Markdown’s formatting syntax is simply to look at a Markdown-formatted document. For example, you can view the Markdown source for the article text on this page here: [http://daringfireball.net/projects/markdown/index.text](https://daringfireball.net/projects/markdown/index.text)

(You can use this ‘.text’ suffix trick to view the Markdown source for the content of each of the pages in this section, e.g. the [Syntax](https://daringfireball.net/projects/markdown/syntax.text) and [License](https://daringfireball.net/projects/markdown/license.text) pages.)

Markdown is free software, available under a BSD-style open source license. See the [License](https://daringfireball.net/projects/markdown/license) page for more information.

## DISCUSSION LIST

I’ve set up a public [mailing list for discussion about Markdown](http://six.pairlist.net/mailman/listinfo/markdown-discuss). Any topic related to Markdown — both its formatting syntax and its software — is fair game for discussion. Anyone who is interested is welcome to join.

It’s my hope that the mailing list will lead to good ideas for future improvements to Markdown.

## INSTALLATION AND REQUIREMENTS

Markdown requires Perl 5.6.0 or later. Welcome to the 21st Century. Markdown also requires the standard Perl library module [Digest::MD5](http://search.cpan.org/dist/Digest-MD5/MD5.pm), which is probably already installed on your server.

### MOVABLE TYPE

Markdown works with Movable Type version 2.6 or later (including Movable Type 3.0).

1.  Copy the “Markdown.pl” file into your Movable Type “plugins” directory. The “plugins” directory should be in the same directory as “mt.cgi”; if the “plugins” directory doesn’t already exist, use your FTP program to create it. Your installation should look like this:
    
    ```
    (mt home)/plugins/Markdown.pl
    ```
    
2.  Once installed, Markdown will appear as an option in Movable Type’s Text Formatting pop-up menu. This is selectable on a per-post basis:
    
    ![Screenshot of Movable Type 'Text Formatting' Menu](https://daringfireball.net/graphics/markdown/mt_textformat_menu.png)
    
    Markdown translates your posts to HTML when you publish; the posts themselves are stored in your MT database in Markdown format.
    
3.  If you also install SmartyPants 1.5 (or later), Markdown will offer a second text formatting option: “Markdown With SmartyPants”. This option is the same as the regular “Markdown” formatter, except that it automatically uses SmartyPants to create typographically correct curly quotes, em-dashes, and ellipses. See the [SmartyPants web page](https://daringfireball.net/projects/smartypants/) for more information.
    
4.  To make Markdown (or “Markdown With SmartyPants”) your default text formatting option for new posts, go to Weblog Config: Preferences.
    

Note that by default, Markdown produces XHTML output. To configure Markdown to produce HTML 4 output, see “Configuration”, below.

### BLOSXOM

Markdown works with Blosxom version 2.0 or later.

1.  Rename the “Markdown.pl” plug-in to “Markdown” (case is important). Movable Type requires plug-ins to have a “.pl” extension; Blosxom forbids it.
    
2.  Copy the “Markdown” plug-in file to your Blosxom plug-ins folder. If you’re not sure where your Blosxom plug-ins folder is, see the Blosxom documentation for information.
    
3.  That’s it. The entries in your weblog will now automatically be processed by Markdown.
    
4.  If you’d like to apply Markdown formatting only to certain posts, rather than all of them, Markdown can optionally be used in conjunction with Blosxom’s [Meta](http://www.blosxom.com/plugins/meta/meta.htm) plug-in. First, install the Meta plug-in. Next, open the Markdown plug-in file in a text editor, and set the configuration variable `$g_blosxom_use_meta` to 1. Then, simply include a “`meta-markup: Markdown`” header line at the top of each post you compose using Markdown.
    

### BBEDIT

Markdown works with BBEdit 6.1 or later on Mac OS X. It also works with BBEdit 5.1 or later and MacPerl 5.6.1 on Mac OS 8.6 or later. If you’re running Mac OS X 10.2 (Jaguar), you may need to install the Perl module [Digest::MD5](http://search.cpan.org/dist/Digest-MD5/MD5.pm) from CPAN; Digest::MD5 comes pre-installed on Mac OS X 10.3 (Panther).

1.  Copy the “Markdown.pl” file to appropriate filters folder in your “BBEdit Support” folder. On Mac OS X, this should be:
    
    ```
    BBEdit Support/Unix Support/Unix Filters/
    ```
    
    See the BBEdit documentation for more details on the location of these folders.
    
    You can rename “Markdown.pl” to whatever you wish.
    
2.  That’s it. To use Markdown, select some text in a BBEdit document, then choose Markdown from the Filters sub-menu in the “#!” menu, or the Filters floating palette
    

## CONFIGURATION

By default, Markdown produces XHTML output for tags with empty elements. E.g.:

```
<br />
```

Markdown can be configured to produce HTML-style tags; e.g.:

```
<br>
```

### MOVABLE TYPE

You need to use a special `MTMarkdownOptions` container tag in each Movable Type template where you want HTML 4-style output:

```
<MTMarkdownOptions output='html4'>
    ... put your entry content here ...
</MTMarkdownOptions>
```

The easiest way to use MTMarkdownOptions is probably to put the opening tag right after your `<body>` tag, and the closing tag right before `</body>`.

To suppress Markdown processing in a particular template, i.e. to publish the raw Markdown-formatted text without translation into (X)HTML, set the `output` attribute to ‘raw’:

```
<MTMarkdownOptions output='raw'>
    ... put your entry content here ...
</MTMarkdownOptions>
```

### COMMAND-LINE

Use the `--html4tags` command-line switch to produce HTML output from a Unix-style command line. E.g.:

```
% perl Markdown.pl --html4tags foo.text
```

Type `perldoc Markdown.pl`, or read the POD documentation within the Markdown.pl source code for more information.

## ACKNOWLEDGEMENTS

[Aaron Swartz](http://www.aaronsw.com/) deserves a tremendous amount of credit for his feedback on the design of Markdown’s formatting syntax. Markdown is _much_ better thanks to Aaron’s ideas, feedback, and testing. Also, Aaron’s [html2text](http://www.aaronsw.com/2002/html2text/) is a very handy (and free) utility for turning HTML into Markdown-formatted plain text.

[Nathaniel Irons](http://bumppo.net/), [Dan Benjamin](http://hivelogic.com/), [Daniel Bogan](http://waferbaby.com/), and [Jason Perkins](http://pressedpants.com/) also deserve thanks for their feedback.

[Michel Fortin](http://www.michelf.com/projects/php-markdown/) has ported Markdown to PHP; it’s a splendid port, and highly recommended for anyone looking for a PHP implementation of Markdown.

 

[Display Preferences](https://daringfireball.net/preferences/ "Customize the font size and presentation options for this web site.")  
  
Copyright © 2002–2021 The Daring Fireball Company LLC.