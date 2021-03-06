#include "./controller/GameController.hpp"
#include "./controller/RootController.hpp"
#include "./controller/StaticController.hpp"
#include "./AppComponent.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {

    /* Register Components in scope of run() method */
    AppComponent components;

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    /* Create GameController and add all of its endpoints to router */
    const std::shared_ptr<GameController> &controller = std::make_shared<GameController>();
    router->addController(controller);
    router->addController(std::make_shared<RootController>());
    router->addController(std::make_shared<StaticController>());

    /* Create SwaggerController and add all of its endpoints to router */
    oatpp::web::server::api::Endpoints docEndpoints;
    docEndpoints.append(router->addController(controller)->getEndpoints());
    router->addController(oatpp::swagger::Controller::createShared(docEndpoints));

    /* Get connection handler component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

    /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
    oatpp::network::Server server(connectionProvider, connectionHandler);

    /* Print info about server port */
    OATPP_LOGI("BlackJack", "Server running on port %s", connectionProvider->getProperty("port").getData());

    TimeoutCleaner::GetInstance();

    /* Run server */
    server.run();

}

/**
 *  main
 */
int main(int argc, const char *argv[]) {

    oatpp::base::Environment::init();

    run();

    /* Print how much objects were created during app running, and what have left-probably leaked */
    /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
    std::cout << "\nEnvironment:\n";
    std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
    std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

    oatpp::base::Environment::destroy();

    return 0;
}
