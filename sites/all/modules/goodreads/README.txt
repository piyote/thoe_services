CONTENTS OF THIS FILE
---------------------

Author
Sponsor
Goodreads Introduction
Goodreads Module
Goodreads Filter Submodule
Goodreads Views Submodule
Goodreads OAuth Submodule
Goodreads OAuth Under the Hood


Author
------
Gregg Marshall
gmarshall@vendor-tech.com
http://drupal.org/user/84148


Sponsor
-------
Development of this module was sponsored by the
Columbus Metropolitan Library
http://www.columbuslibrary.org/


Goodreads Introduction
----------------------

Goodreads (http://www.goodreads.com) is a social networking website for readers
and claims to be the largest site for book recommendations.  As of 2016,
Goodreads had 55 million members who added more than 1.5 billion books to
their “shelves.”   Goodreads members have created about 50 million
reviews(which can be as short as a star rating) according to Read Write Web
(http://www.readwriteweb.com/archives/
    goodreads_book_recommendation_engine_launched.php).
A lot of the book data itself comes from a number of sources, including Amazon,
Barnes and Noble, Ingram, Library of Congress, Worldcat and others along with
data submitted by individual publishers and Goodreads users that become
librarians.

This module, along with its bundled submodules, let you integrate that
Goodreads data onto your Drupal website.

Goodreads book reviews are the obvious data most Drupal websites are likely
to want to integrate.  Goodreads recently changed how those reviews are
displayed, replacing the XML review text with an iFrame display of reviews for
a given book (one presumes to protect their intellectual property).  Single
reviews can still be accessed via the API, although it is relatively complex.
In theory, with special permissions, the older XML versions of the reviews
can be accessed, but that is outside the scope of this module at this time.

In addition to seeing the reviews, and other associated book data, the
Goodreads API allows you to organize a user’s books onto user defined
bookshelves, create and edit reviews, and see the connections of various
editions, called works by Goodreads, along with their linkages to authors and
book series.

Beyond book data and reviews, Goodreads is first and foremost a community,
so it has the usual social networking functions:  you can request to be a
friend, you can follow someone’s reviews and status updates, you can join
groups and post messages on them, much like Drupal’s forums, and you can
comment on virtually anything. All this social interaction can be supported
y the Goodreads API.

This module, with its submodules, implements most of the Goodreads API.
The only part of the API not implemented are those calls that require
additional permissions beyond what you can get by signing up as a developer.
In addition, there are a few API calls that don’t work, mostly due to now
known bugs, which hopefully will get fixed in the near future.  Of the 56
documented Goodreads APIs, this base Goodreads module exposes 27, the OAuth
module adds 26, 4 currently have unresolved API issues, and there are 4 that
require extra permissions and not implemented.  The numbers don’t add up
because some API’s have OAuth options, resulting in two PHP interface
functions and the non-functioning API’s are included in the other categories.

The Goodreads module itself is mostly an API support module, and provides the
UI to enter your Goodreads developer key information.  It also provides
cacheing for the non-OAuth , which could be a requirement to stay within
the Goodreads API call limits.

The Goodreads Filter module exposes Goodreads data to your Drupal content
via a new input filter which adds tokens like
[goodreads_get_isbn_reviews title 1430209895]
to insert the title of a book with ISBN 1430209895 into your content.
Using a somewhat unusual notation, this module will let you access virtually
anything in the Goodreads API.  Additionally, in theory it would be possible
to use the OAuth module’s functions to change things on Goodreads using the
filter, although it is highly discouraged!

The Goodreads Views module stores information about each users’ Goodreads
books on all their shelves in a table in your Drupal database and makes that
information available via the Views module.  The data is rebuilt via cron at
a configurable time each day using the Drupal Queue module to avoid overloading
your cron job.  Depending on the number of Goodreads users, and the number of
books on their shelves, you may need several cron runs a day to keep the table
up to date.

The Goodreads OAuth module adds support for using the OAuth module’s libraries
to enable access to those Goodreads APIs which require OAuth authorization to
utilize a user’s information.  It adds additional API functions and extends
the other submodules to expose additional data to your Drupal content.  In
addition, if you want to change anything on the Goodreads side, you will need
the Goodreads OAuth module.

We'd like to thank the developers of the Amazon module
(http://drupal.org/project/amazon) for the inspiration of how to expose the
Goodreads data to Drupal.  Because a lot of Goodreads book data comes from
Amazon, and the associated restrictions that Amazon puts on redistribution
of that data, Goodreads doesn’t always supply all the information that is
displayed on their site via the API.  You may find you need the Amazon module
to directly access that data from Amazon.

This module was developed for Columbus Metropolitan Library. The use case for
Columbus Metropolitan Library uses just a fraction of the Goodreads API
exposed by the module. I expect that as more people use it we’ll find lots
of areas for improvement.


Goodreads Module
----------------

Alone, the main Goodreads module is not going to be useful to a Drupal site
builder that cannot program in PHP.  While it has a configurations page,
beyond that its purpose is to expose those Goodreads APIs that do not use
OAuth as a set of PHP functions that can be called from within a custom module.

To install the Goodreads module:
1. Download the Goodreads module from the project page.  Copy the expanded
folder, along with its files and subfolders, into your modules directory
(most likely sites/all/modules/contrib or sites/all/modules).
2. Go to the Goodreads site and register for developer API keys for your
website.  The direct link to the registration page is
http://www.goodreads.com/api/keys.  Write down your key and secret for later
(and/or keep the page open in another window).
3. Enable the Goodreads module on your site’s module page
(Administer >> Site building >> Modules).
4. Enter your Goodreads developer keys at the Goodreads settings page
(Administer >> Site configuration >> Goodreads API).  Select how long you want
to cache the Goodreads data, you can select from no cacheing up to one day,
the limit imposed by the Goodreads API terms and conditions.  Picking too
long a cache time will prevent rapidly changing information on the Goodreads
site from being reflected on your site, picking too short a cache time could
put your site in violation of the one API call per second limit imposed by the
Goodreads API terms and conditions or substantially affect your site’s
performance.
5. Click on the test tab on the Goodreads setting page.  Enter an ISBN, the
default is for Pro Drupal Development, and click the “Look up book” button.
If you entered your Goodreads API keys correctly, the page should display the
iFrame reviews widget for the book, followed by a var_dump of the API results
returned by that function.  If an error occurred, it is logged in the Drupal
watchdog log.
You are set to start using the functions provided by the Goodreads module,
or enable one of the submodules to use the Goodreads data without a custom
module.

***** A Security Note *****
None of the Goodreads module(s) makes any attempt to sanitize the results
coming back from Goodreads via XML.  Some of the results are plain text,
some of the results are HTML, and the reviews widget includes an iFrame
and embedded style definitions.  That variability, coupled with the very
large number of results options that might or might not be returned makes
attempting to sanitize them impractical.  If you don't trust the Goodreads
website to return safe content, you might consider sanitizing the results you
actually use with check_plain() or filter_xss().


Goodreads Filter Submodule
--------------------------

This submodule exposes the data from the Goodreads module (and optionally
the Goodreads OAuth module) to Drupal via an input filter.
To use the Goodreads filter submodule:
1. Make sure you have the main Goodreads module installed and configured.
2. Enable the Goodreads filter submodule on your site’s module page
(Administer >> Site building >> Modules).
3. The Goodreads filter submodule needs to be added to one or more existing
input filters, such as the full HTML input filter
(Administer >> Site configuration >> Input formats), select the Goodreads
filter checkbox, and save your settings.

Once you’ve configured the an input filter to include the Goodreads filter,
you can insert Goodreads data using the general format
   [goodreads_function_name selector parameters]
Square brackets are used to delimit a Goodreads token, like many other
contributed module input filters.
After the opening square bracket insert the Goodreads API function name
you want to access.  All the Goodreads API PHP functions start with
“goodreads_” and are listed alphabetically.  As an example, to get
information about a book using its ISBN, the corresponding Goodreads
API function would be:
   goodreads_get_isbn_reviews($isbn, $rating = '', $text_only = '')
The input filter for that function would start:
   [goodreads_get_isbn_reviews ….]
After the function name is the data selector for the data element you want.
Each Goodreads API function will return a different multi-dimensional array
with the data returned by that Goodreads API call.  You will need to use the
sample XML displays from the Goodreads API (http://www.goodreads.com/api) or
the sample selector list contained in GOODREADS_FILTER_LIST.txt in the
Goodreads Filter folder to pick the selector you need.
Selector notation changes the format of standard PHP array notation to avoid
conflicting with the square brackets used by many token input filters,
including the Goodreads filter submodule.  To convert from standard PHP array
notation, remove the square brackets and any quotations from the associative
array and separate the elements with hyphens.  In the continuing example of
the goodreads_get_isbn_reviews function, if the result is returned in a
multi-dimensional array $reviews, to represent the first author of a book,
found at
   $reviews[‘authors’][‘author’][0][‘name’]
You would use the selector authors-author-0-name, separated from the function
name by a space like:
   [goodreads_get_isbn_reviews authors-author-0-name …]
In the selector list, a number of selectors have a 0 shown.  When a Goodreads
API can return more than one result for a given element, they are contained in
an indexed array selected by their number: 0, 1, 2, etc.  So to get the
primary author for a book use the above example, to get the second author of
a book use:
   [goodreads_get_isbn_reviews authors-author-1-name …]
While getting a single value of the group might work for many applications,
if you would like to get all the values, separated by commas, substitute an
asterisk for the index number like:
   [goodreads_get_isbn_reviews authors-author-*-name …]
The last parts of the Goodreads filter module token are any parameters that
are to be passed to the function.  They are separated from the selector with
a space.  Continuing our example in the simplest case, the
goodreads_get_isbn_reviews function needs the ISBN of the book.
For this example we’ll use 1430209895, the ISBN of Pro Drupal Development:
   [goodreads_get_isbn_reviews authors-author-0-name 1430209895]
Which would insert
   John K. VanDyk
into the content you put this token into. If you see the token and not the
result, the most likely cause is you haven’t selected the proper input format,
or that format doesn’t have the Goodreads filter enabled.
Many functions have multiple parameters, separate them with commas.  Any blank
parameter can be skipped when needed.  For the goodreads_get_isbn_reviews
function if we only want to return reviews that contain text, therefore
excluding any review that is just a five star rating, for Pro Drupal
Development, the token to for the reviews iFrame widget would be
   [goodreads_get_isbn_reviews reviews_widget 1430209895,,true]
A more complex example that returns the first page members of a Goodreads
group, the Goodreads developer group, sorted by number of posts/comments
would be
   [goodreads_get_group_members group_user-*-user-first_name
   62646,1,num_comments,d]
Spaces following a comma are ignored.  Strings can be enclosed in single or
double quotes, or not at all, as shown in the above example.
To review, the general format for a Goodreads filter token is function,
selector and parameters:
   [goodreads_function_name selector parameters]
In theory any of the Goodreads API functions can be used in a Goodreads filter
token, including the OAuth API’s if the Goodreads OAuth submodule is enabled.
While that suggests it would be possible to insert a Goodreads filter token
that would change something on the Goodreads site, such as posting a review
to a book, it is not suggested.  We can’t think of a use case and worry about
submitting the same change multiple times as a page gets loaded over and over.
Also it is sort of possible to pass the results of one Goodreads filter token
as the last parameter of another Goodreads filter token.  It wasn’t in the
original design but appears to be possible, with some limitations, because
the filter can be called recursively.  However, at least as currently
implemented, the nested token must be the last parameter of the outer token
because of the way the parameter string is parsed by the regex expression
in the filter.  In addition, for the same reason, the syntax is slightly off;
only one closing square bracket is used.  To get the names of all the groups
the current user belongs to you could use
   [goodreads_get_user_groups list-group-*-title
   [goodreads_oauth_get_user_id id]
This has some use as it is implemented but really needs a patch contributed
to make it work in a more general and symmetrical manner.


Goodreads Views Submodule
-------------------------

This submodule implements a common use Goodreads data integration using Views.
Once a day during a cron job, the books on every shelf for every Drupal user
that has a corresponding Goodreads user ID is  is read into a MySQL table in
Drupal.

The Goodreads views submodule currently uses the Goodreads OAuth submodule
primarily as its way of knowing that Goodreads user ID is associated with
each Drupal UID.  In addition it uses the OAuth version of the Goodreads
get the books on a user’s shelf API call to access a complete listing of
books on all shelves, overriding the visibility settings that might hide
bookshelves depending on how the user has set their Goodreads privacy options.
It would be relatively straightforward to create a non-OAuth alternative.

To use the Goodreads views submodule
1. Make sure you have the main Goodreads module installed and configured,
   as well as the Goodreads OAuth submodule, along with the Views module,
   and any of its required modules, and the Drupal Queue module.
2. Enable the Goodreads views submodule on your site’s module page
   (Administer >> Site building >> Modules).
3. Configure the maximum number of books and when to start the cron update
   at the Goodreads views settings page
   (Administer >> Site configuration >> Goodreads API >> Views).  Some
   Goodreads users can have a large number of books, organized on a large
   number of shelves. For many applications, the performance overhead of
   downloading hundreds, or thousands, of books for many bookshelves isn’t
   desirable.  The number per bookshelf can be limited using the first setting.
   The Goodreads views submodule updates its table once a day, using the
   Drupal Queue module’s queuing to avoid cron timeouts.  You can select
   what time of day to schedule the Goodreads views cron job using a pull
   down selection.  Your cron should be set up to run often enough to allow
   the Drupal Queue module to process all the Goodreads views jobs scheduled
   each day.  As a rule of thumb, depending on how many books each user has,
   approximately 5-10 users are processed per cron run.  Depending on how
   many users you have for Goodreads views, you may need to configure the
   Drupal Queue module’s separate cron job option to be processed much more
   frequently than your site’s regular cron job.

From the Goodreads views submodule configuration page you can force the
scheduling of a Goodreads views cache refill, which still requires the how
ever many cron runs necessary to process the users, or clear the data in the
Goodreads views cache table, which deletes all the Goodreads views data.

Once the cron run has populated the Goodreads views data table, the following
fields are available to views:
• User ID from {user}.uid.
• Cache time, the date and time the cache data was created.
• Goodreads user ID
• Shelf name, the Goodreads shelf this book is on.
• Title, the title of book.
• Author, the primary author of the book.
• Ratings count, the number of ratings for this book.
• Average rating, the average rating for this book.
• Text reviews count, the number of text reviews.
  (reviews with more than the five star rating)
• Image URL, the URL of a book image.  Note that a high percentage of the book
  images shown on the Goodreads site are restricted from distribution via the
  API.  You may find you need the Amazon module to display book images or use
  LibraryThing's database of book covers
  (http://www.librarything.com/blogs/librarything/2008/08/
  a-million-free-covers-from-librarything/)
• ISBN, the ISBN for the book.
• Goodreads book ID.
• Description, the description of the book.

The table is defined as a base table, which means you can create a view type
of Goodreads Book Data, which is listed at the bottom of the add view page.

The table is also related to Drupal's User type, allowing relationships from
either a User view type to Goodreads book data, or from a Goodreads book data
view type to user fields.  The relationship is selected by adding a
relationship from views third column, Relationships, and selecting
Goodreads Book Data: Drupal User ID.


Goodreads OAuth Submodule
-------------------------

This submodule implements additional Goodreads API calls that require OAuth
to access a specific Goodreads user’s information.

OAuth can be a very intimidating addition to a module.  It needn’t be.

In its simplest, OAuth is a way for your website (a Consumer) to get,
or in some changes change, more/less private information from another website
(a Provider) on behalf of a User, without that User giving you their username
or password for the Provider website.

The process OAuth uses to accomplish that is byzantinely complex internally,
but for the average user of this module you won’t have to know a whole lot
about how it accomplishes the magic.  The process uses the same developer key
and developer secret you need for the base Goodreads module, so there isn’t
any special configuration required.

When you enable this submodule, each user will get a new capability to link
their Goodreads account to your website, once linked they won’t have to ever
answer that question again.  You’ve seen OAuth in action on Facebook, Twitter,
etc when at some point you see a page on those sites that says “Authorize xyz
to use your account” or “xyz wants access to your account.”  For instance,
when a Drupal user clicks links their account on your website to their
Goodreads account, they will see a Goodreads page asking if they want to allow
your website access to their information.

Once that User allows access for your site to access their Goodreads account,
the submodule stores a special access token that can be used to automatically
identify your site and the fact the User has given you permission to interact
with their Goodreads account.
In more detail, the steps required to use the Goodreads OAuth API’s are:
1. Register as a Goodreads developer at http://www.goodreads.com/api/keys.
You need to be logged into Goodreads (which obviously means you need to join
Goodreads).  When you register, you will be given your developer key and
secret.  Enter these into the Goodreads module settings page.  You needed to
do this step to use any Goodreads API.
2. Install and enable the Goodreads OAuth module.  The Goodreads OAuth module
will add a tab to the user profile page to allow a user to link their Drupal
account to their Goodreads account.  The module also creates a link,
/goodreads/user/authorize, that you can use to start the Goodreads OAuth
process in other places.
3. When the link Goodreads account button or link is clicked, the module will
redirect the user’s browser to http://www.goodreads.com/authorize page with
the request token as a URL parameter.  That will display a page on the
Goodreads site for the user to Allow access.
4. Assuming the user allows access, the user's Goodreads user ID, access
token and access secret are stored in the Goodreads OAuth token table for
later use by any Goodreads API functions that require OAuth authorization.



Goodreads OAuth Under the Hood
------------------------------

If you are curious about what the Goodreads OAuth submodule is doing under the
hood, read on.  None of the information should be critical to just use the
module.

If you aren’t familiar with OAuth, it was designed as an industry standard
method for one website (a Provider) to share a User’s information with another
website (a Consumer) without sharing the User’s Provider site’s username and
password with the Consumer site, a sort of authentication Ménage à trios.

The way it accomplishes this relatively straightforward process involves keys,
tokens, state machines, all designed to make the process secure, even across
unencrypted linkages, since most web services are not protected by SSL.  Like
most web services, often knows as web APIs, the process involves what looks
like “calls” to specific web pages which usually return XML or JSON instead
of the HTML/CSS that we normally think of.  So instead of a user typing a URL
into their web browser and getting back a web page to look at, your website
is sending a more complex URL to another website and getting back the answer
in XML or JSON so it can be more easily processed by a program, your web site.

Because the spec is designed to keep your information secure, even without
encrypted links, the protocol is quite complex requiring several exchanges
of keys and tokens to ensure that the Consumer website is who they say they
are, and that the User has authorized the sharing of their information.

For a much better, and more comprehensive description of how OAuth works,
I’d recommend you read the short article “OAuth for Dummies,” at
http://marktrapp.com/blog/2009/09/17/oauth-dummies.  Once you have mastered
that excellent tutorial, the most commonly referenced tutorial on OAuth is
The OAuth 1.0 Guide, found at http://hueniverse.com/oauth/guide/ .
Finally there is the Drupal documentation page OAuth at
http://drupal.org/node/349516, which is good although I wish the author
had just focused on the current OAuth module’s implementation and skipped
the cool-auth section.  It also focuses on the web services data being passed
than the PHP API calls.  And if you really are interested the OAuth standard
is at http://oauth.net/.

In more detail, the steps required to use the Goodreads OAuth API’s are:
1. Register as a Goodreads developer at http://www.goodreads.com/api/keys.
   You need to be logged into Goodreads (which obviously means you need to
   join Goodreads).  When you register, you will be given your developer key
   and secret.  Enter these into the Goodreads module settings page.  You
   needed to do this step to use any Goodreads API.
2. Install and enable the Goodreads OAuth module.  The Goodreads OAuth module
   will add a tab to the user profile page to allow a user to link their Drupal
   account to their Goodreads account.  The module also creates a link,
   /goodreads/user/authorize, that you can use to start the Goodreads OAuth
   process in other places.
3. When the link Goodreads account button or link is clicked, the module will
   get a request token from Goodreads using the developer keys from step #1
   at the http://www.goodreads.com/request_token API URL.
4. Then the Goodreads OAuth module will redirect the user’s browser to
   http://www.goodreads.com/authorize page with the request token as a URL
   parameter.  That will display a page on the Goodreads site for the user
   to Allow access.
5. When the user has allowed access, Goodreads will redirect back to a
   callback URL given by the Goodreads OAuth module with the authorization
   token and the parameter “authorized=1.”
6. The Goodreads OAuth module will exchance that authorization token for a
   “permanent” access token using the http://www.goodreads.com/access_token
   API URL.  That access token, along with the user’s Goodreads user ID,
   will be stored in a goodreads_access_token table created when the module
   is enabled and used in all further OAuth required API calls.
