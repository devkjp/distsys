#!/usr/bin/perl

use strict;
use warnings;

use Test::More;
use IO::Socket::IP;


my $root_dir    = "web";
my $remote_host = "localhost";
my $remote_port = "8080";


#--------------------------------------------------------------------------
# Test Cases
#--------------------------------------------------------------------------
my @tests = (
    [ "GET",      200 ],
    [ "HEAD",     200 ],
    [ "OPTIONS",  501 ],
    [ "POST",     501 ],
    [ "PUT",      501 ],
    [ "DELETE",   501 ],
    [ "TRACE",    501 ],
    [ "CONNECT",  501 ],
    [ "DUMMY",    400 ]
);

plan tests => scalar @tests;

connect_to_server(@$_) for @tests;

# List all modules included by this script
#print join("\n", map { s|/|::|g; s|\.pm$||; $_ } keys %INC);

exit 0;


sub connect_to_server {
    my $method = shift;
    my $status = shift;

    my $socket = IO::Socket::IP->new(
                PeerAddr => $remote_host,
                PeerPort => $remote_port,
                Type     => SOCK_STREAM
    ) or die "ERROR: socket() - $@";

    print $socket "$method / HTTP/1.0\r\n\r\n";

    my $answer = <$socket> =~ s/\R\z//r;
    my @fields = split " ", $answer;
    is($fields[1], $status, "Method $method: $status");

    close($socket);
} # end of connect_to_server

