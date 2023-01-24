#include "Server.hpp"

int	main(int ac, char **av) {

	if (ac != 3)
	{
		std::cout << "wrong number of arguments" << std::endl;
		exit(0);
	}
	try
	{
		Server	server(av[1], av[2]);	
		server.launch_serv();
		while (server.get_run())
			server.wait_for_client();
		return (0);
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}
}
