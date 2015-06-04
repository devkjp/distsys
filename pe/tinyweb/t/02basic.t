#!/usr/bin/perl

use strict;
use warnings;

use File::stat;
use File::MimeInfo;
use POSIX qw(strftime);
use POSIX::strptime;
use POSIX qw(tzset);
use LWP::UserAgent;
use Test::More;
use Data::Dumper;

my $root_dir    = "web";
my $remote_host = "localhost";
my $remote_port = "8080";
my $remote_path = "";

#my $remote_host = "ralfreutemann.name";
#my $remote_port = "80";
#my $remote_path = "/distsys";


my %http_status = (
    200 => "OK",
    206 => "Partial Content",
    301 => "Moved Permanently",
    304 => "Not Modified",
    400 => "Bad Request",
    403 => "Forbidden",
    404 => "Not Found",
    416 => "Requested Range Not Satisfiable",
    500 => "Internal Server Error",
    501 => "Not Implemented"
);


#--------------------------------------------------------------------------
# Test Cases
#--------------------------------------------------------------------------
my @tests = (
    [ { method => 'HEAD', url => "/index.html", status => 200 } ],
    [ { method => 'GET',  url => "/index.html", status => 200 } ],
    [ { method => 'HEAD', url => "/images/computerhead1.gif", status => 200 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 200 } ],
    [ { method => 'HEAD', url => "/example.pdf", status => 200 } ],
    [ { method => 'GET',  url => "/example.pdf", status => 200 } ],
    [ { method => 'HEAD', url => "/css/default.css", status => 200 } ],
    [ { method => 'GET',  url => "/css/default.css", status => 200 } ],
    # Non-existing file
    [ { method => 'HEAD', url => "/blablabla.html", status => 404 } ],
    [ { method => 'GET',  url => "/blablabla.html", status => 404 } ],
    # No permission to read file
    [ { method => 'HEAD', url => "/forbidden.html", status => 403 } ],
    [ { method => 'GET',  url => "/forbidden.html", status => 403 } ],
    # No permission to read directory
    [ { method => 'HEAD', url => "/forbidden", status => 403 } ],
    [ { method => 'GET',  url => "/forbidden", status => 403 } ],
    # Redirection to default file
    [ { method => 'HEAD', url => "/", status => 200, file => "/default.html" } ],
    [ { method => 'GET',  url => "/", status => 200, file => "/default.html" } ],
    # Redirection in case of a directory
    [ { method => 'HEAD', url => "/source", status => 301, location => "/source/" } ],
    [ { method => 'GET',  url => "/source", status => 301, location => "/source/" } ],
    # If-Modified-Since
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 304, mod_offset => 0 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 304, mod_offset => +1 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 200, mod_offset => -1 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 304, mod_offset => +3201 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 200, mod_offset => -3201 } ],
    # Ranges
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 416, range_offset => -1 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 416, range_offset => 100000 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 206, range_offset => 0 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 206, range_offset => 1 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 206, range_offset => -2 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 206, range_offset => -500 } ],
    [ { method => 'GET',  url => "/images/computerhead1.gif", status => 206, range_offset => 500 } ]
);

# Set the number of test cases (excluding subtests)
plan tests => scalar @tests;

# Force the time zone to be GMT
$ENV{TZ} = 'GMT';
tzset;

connect_to_server(@$_) for @tests;

exit 0;


sub check_date_header {
    my $date_header = shift;

    # Get the current system time in GMT
    my @tm_now = gmtime;
    my $time_now = POSIX::mktime(@tm_now);

    # Parse the server date header and translate into a time value, ignore the DST field
    # which may or may be not initialised depending on the host system.
    my @date_fields = (POSIX::strptime $date_header, "%a, %d %b %Y %H:%M:%S")[0,1,2,3,4,5,6];
    my $time_response = POSIX::mktime(@date_fields);

    # Finally, compare the two time values...
    my $delta = abs($time_response - $time_now);
    ok($delta <= 1, "Date") or print "Note: Time difference is $delta sec.\n";
} # end of check_date_header


sub get_properties_for_url {
    my $ref = shift;

    my $file = $root_dir . ((exists $ref->{file}) ? $ref->{file} : $ref->{url});
    my $st = stat($file) or die "ERROR: cannot access $file: $!";
    my $file_time = strftime "%a, %d %b %Y %H:%M:%S GMT", gmtime $st->mtime;

    return ($file, $file_time, $st->size);
} # end of get_properties_for_url


sub check_file_content {
    my $content = shift;
    my $file    = shift;
    my $offset  = shift;

    my $st = stat($file) or die "ERROR: cannot access $file: $!";

    $offset = 0 unless defined $offset;

    # Compare the length of the response body with the size of the requested range 
    is(length($content), $st->size - $offset, "Response body length");

    # Read the contents of the file and compare it with the contents
    # of the response body. Read the file in one go and not line by
    # line, as it is the default.
    my $resource = do {
        local $/ = undef;
        open my $fh, "<", $file
           or die "ERROR: cannot open $file: $!";
        seek $fh, $offset, 0 if $offset > 0;
        <$fh>;
    };
    ok($content eq $resource, "Response body content");
}

sub connect_to_server {
    my $ref = shift;

    my $method = $ref->{method};
    my $url = $ref->{url};
    my $status = $ref->{status} . " " . $http_status{$ref->{status}};
    my $offset = undef;

    # Create a user agent object
    my $ua = LWP::UserAgent->new(max_redirect => 0, timeout => 30);
    $ua->agent("TinyWeb Test Harness, Test Script $0");

    # Create a request
    my $req = HTTP::Request->new($method => "http://$remote_host:$remote_port$remote_path$url");
    $req->header('Accept' => '*/*');
    if (exists $ref->{mod_offset}) {
        my $st = stat($root_dir . $url) or die "ERROR: cannot access $url: $!";
        $req->header('If-Modified-Since'
             => strftime "%a, %d %b %Y %H:%M:%S GMT", gmtime ($st->mtime + $ref->{mod_offset}));
    } # end if
    if (exists $ref->{range_offset}) {
        my $st = stat($root_dir . $url) or die "ERROR: cannot access $url: $!";
        $offset = ($ref->{range_offset} < 0) ?
                   $st->size + $ref->{range_offset} + 1 : $ref->{range_offset};
        $req->header('Range' => "bytes=$offset-");
    } # end if

    # Pass request to the user agent and get a response back from the server
    my $res = $ua->request($req);

    subtest "$method '$url'" => sub {
        #--------------------------------------------------
        # Subtest: HTTP Status is as expected
        #--------------------------------------------------
        is($res->status_line, $status, "Status");

        #--------------------------------------------------
        # Subtest: Date and time is correct
        #--------------------------------------------------
        check_date_header($res->headers->{'date'});

        #--------------------------------------------------
        # Subtest: Header field 'Connection' is provided
        #--------------------------------------------------
        isnt($res->headers->{'connection'}, undef, "Connection");

        #--------------------------------------------------
        # Subtest: Header field 'Server' is provided
        #--------------------------------------------------
        isnt($res->headers->{'server'}, undef, "Server");

        if (($ref->{status} == 200) || ($ref->{status} == 206)) {
            #------------------------------------------------------------------
            # Status: 200 OK or 206 Partial Contents
            #------------------------------------------------------------------

            # Determine file properties
            (my $file, my $file_time, my $file_size) = get_properties_for_url($ref);

            #------------------------------------------------------------------
            # Subtest: Header field 'Last-Modified' equal to file mtime
            #------------------------------------------------------------------
            is($res->headers->{'last-modified'}, $file_time, "Last-Modified");

            #------------------------------------------------------------------
            # Subtest: Header field 'Content-Length' equal to file size
            #------------------------------------------------------------------
            my $exp_size = (defined $offset) ? $file_size - $offset : $file_size;
            is($res->headers->{'content-length'}, $exp_size, "Content-Length");

            #------------------------------------------------------------------
            # Subtest: Header field 'Content-Rage'
            #------------------------------------------------------------------
            my $range_str = undef;
            if (defined $offset) {
                $range_str = sprintf "bytes %d-%d/%d", $offset, $file_size-1, $file_size;
            } # end if
            is($res->headers->{'content-range'}, $range_str, "Content-Range");

            #------------------------------------------------------------------
            # Subtest: Header field 'Accept-Ranges' is provided
            #------------------------------------------------------------------
            is($res->headers->{'accept-ranges'}, "bytes", "Accept-Ranges");

            #------------------------------------------------------------------
            # Subtest: Header field 'Content-Type' matches file type
            #------------------------------------------------------------------
            is($res->headers->{'content-type'}, mimetype($file), "Content-Type");

            #------------------------------------------------------------------
            # Subtest: Provided response body matches file content
            #------------------------------------------------------------------
            check_file_content($res->content, $file, $offset) if $method eq 'GET';
        } elsif ($ref->{status} == 301) {
            #------------------------------------------------------------------
            # Status: 301 Moved Permanently
            #------------------------------------------------------------------
            my $regex = qr/$ref->{location}$/;
            #------------------------------------------------------------------
            # Subtest: Header field 'Location' contains redirection link
            #------------------------------------------------------------------
            like($res->headers->{'location'}, $regex, "Location");
        } else {
            #------------------------------------------------------------------
            # Status: any other
            #------------------------------------------------------------------
            # do nothing, there are no additional sub-tests
            ;
        } # end if
    };
} # end of connect_to_server

