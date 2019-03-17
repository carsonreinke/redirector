use Test::Nginx::Socket 'no_plan';
use File::Spec;
use Cwd;
use Cwd 'abs_path';

my $hosts_file = abs_path('./tests/resolv-mock.conf');
env_to_nginx("RESOLV_WRAPPER_HOSTS=$hosts_file", 'RESOLV_WRAPPER_DEBUGLEVEL=4');
workers(2);
repeat_each(2);
run_tests();

__DATA__
=== TEST 1: valid
--- config
redirector example.com;
--- request
GET /
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/

=== TEST 2: missing
--- config
redirector missing.example.com;
--- request
GET /
--- response_body_like: Not Found
--- error_code: 404
--- response_headers
! Location

=== TEST 3: location
--- config
redirector example.com;

location /test {
}
--- request
GET /test
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/test

=== TEST 4: override location
--- config
redirector example.com;

location /test {
    return 200;
}
--- request
GET /test
--- error_code: 200
--- response_headers
! Location

=== TEST 5: server variable
--- config
redirector $server_name;
--- request
GET /
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/

=== TEST 6: invalid path
--- config
redirector example.com;
--- request
GET http://example.net/
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/

=== TEST 7: valid query string
--- config
redirector example.com;
--- request
GET /?testing=it
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/?testing=it