# redirector

Nginx static module for redirection from DNS.  Redirects a domain based on proper DNS entry of the domain.

## Why?

'cause.

## Dependencies

* [uriparser](https://uriparser.github.io/)
* [libidn2](https://www.gnu.org/software/libidn/#libidn2)

## Installation

When configuring Nginx just add `--add-module=[REDIRECTOR PATH]` ([see](https://www.nginx.com/resources/wiki/extending/compiling/)).

## Usage

Within the Nginx `server` configuration add `redirector [HOST NAME];`.

The `HOST_NAME` must point to the server and have one of the following:
* TXT record under `_redirect.[HOST NAME]` containing `uri=[URI]` ([RFC1464](https://tools.ietf.org/html/rfc1464) format)
* [URI](https://tools.ietf.org/html/rfc7553) record under `[HOST NAME]` containing `[URI]`

Puny-code domains and DNS entries are supported.  `www` subdomain will automatically be assumed to be the root domain.  URIs are relatively transformed using [RFC3986 Section 5.2.2](https://tools.ietf.org/html/rfc3986#section-5.2.2). 

Below are some examples of what would be returned:

|DNS Entry (for example.com)|Requested URI|Response URI|
|--|--|--|
|`uri=http://example.org`|http://example.com/|http://example.org/|
|`uri=http://example.org`|http://www.example.com/|http://example.org/|
|`uri=http://example.org?query=string`|http://example.com/|http://example.org/|
|`uri=http://example.org?query=string`|http://example.com/?other=string|http://example.org/?other=string|
|`uri=http://example.org/path`|http://example.com/|http://example.org/|
|`uri=http://example.org/path`|http://example.com/otherpath|http://example.org/otherpath/|
|`uri=//example.org`|http://example.com/|`404 Not Found`|

## Testing

Testing is accomplished is two separate pieces either non-Nginx (`libcheck`) or Nginx (`Test::Nginx`).  Both of these tests require use of `resolv_wrapper` to mock DNS responses.  There are four shell scripts to assist with executing the test cases:

* `test-all.sh [RESOLV WRAPPER FILE] [NGINX PATH]` - run all test cases
* `test.sh [RESOLV WRAPPER FILE]` - run non-Nginx test cases
* `test-valgrind.sh [RESOLV WRAPPER FILE]` - run non-Nginx test cases under Valgrind
* `test-nginx.sh [RESOLV WRAPPER FILE] [NGINX PATH]` - run Nginx test cases

The shell scripts require both a path to `resolv_wrapper` and Nginx download.

### Dependencies

* [automake](https://www.gnu.org/software/automake/)
* [libcheck](https://libcheck.github.io/check/)
* [resolv_wrapper](https://cwrap.org/resolv_wrapper.html) - requires a [branch that supports TXT records](https://github.com/carsonreinke/resolv_wrapper/tree/rr_txt)
* [Test::Nginx](https://github.com/openresty/test-nginx)
* [Valgrind](http://www.valgrind.org/)

## Demo

See [https://redirector.website](https://redirector.website)

## Contributing

Bug reports and pull requests are welcome on GitHub at [https://github.com/carsonreinke/redirector](https://github.com/carsonreinke/redirector).

## License

The Nginx module is available as open source under the terms of the [MIT License](http://opensource.org/licenses/MIT).