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
=== valid
--- config
redirector example.com;
--- request
GET /
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/

=== missing
--- config
redirector missing.example.com;
--- request
GET /
--- response_body_like: Not Found
--- error_code: 404
--- response_headers
! Location

=== location
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

=== override location
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

=== server variable
--- config
redirector $server_name;
--- request
GET /
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/

=== invalid path
--- config
redirector example.com;
--- request
GET http://example.net/
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/

=== valid query string
--- config
redirector example.com;
--- request
GET /?testing=it
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/?testing=it

=== punycode
--- config
redirector ✔️.example.com;
--- request
GET /
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: http://example.org/

=== default
--- config
--- request
GET /
--- response_body_like: It works!
--- error_code: 200
--- response_headers
! Location

=== override
--- config
redirector example.com;

location /tests/ {
    root ../../tests/html;
    index index.html;
}
--- request
GET /tests/
--- error_code: 200
--- response_headers
! Location

=== query_string
--- config
redirector qs.example.com;
--- request
GET /
--- response_body_like: Moved Permanently
--- error_code: 301
--- response_headers
Location: https://example.com/