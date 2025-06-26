#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include <boost/asio.hpp>

using namespace boost;

int main(int argc, char* argv[])
{
	if(argc!=2) {
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}

	// Step 1. Assume that the client application has already obtained
	// the DNS name and protocol port number and represented them as 
	// strings.
	//std::string host = "samplehost.com";
	//std::string port_num = "3333";
	std::string host = argv[1];
	std::string port_num = "";

	// Step 2.
	asio::io_context io;

	// Step 3. Creating a query.
	asio::ip::tcp::resolver::query resolver_query(host,
		port_num, 
        asio::ip::tcp::resolver::query::numeric_service);

	// Step 5. Creating a resolver.
	asio::ip::tcp::resolver resolver(io);

	// Used to store information about error that happens
	// during the resolution process.
	system::error_code ec;

	// Step 6.
    auto results = resolver.resolve(resolver_query, ec);

	// Handling errors if any.
	if (ec.value() != 0) {
		// Failed to resolve the DNS name. Breaking execution.
		std::cout << "Failed to resolve a DNS name. Error code = "
			<< ec.value() << ". Message = " << ec.message();

		return ec.value();
	}

    bool once = false;
    for (const auto& entry : results) {
		// Here we can access the endpoint like this.
        if (!once) {
            std::cout << "  Host name: " << entry.host_name() << "\n";
            std::cout << "  -------------------------\n";
            once = true;
        }
        std::cout << "  Address: " << entry.endpoint().address().to_string() << "\n";

        // 判断是否是别名 (如果与查询的host不同)
        if (entry.host_name() != host) {
            std::cout << entry.host_name() << "  --> This is an alias (CNAME)\n";
        }

        std::cout << "  Record type: "
            << (entry.endpoint().address().is_v6() ? "AAAA" : "A")
            << " record\n";
        std::cout << "  -------------------------\n";
    }

	return 0;
}
