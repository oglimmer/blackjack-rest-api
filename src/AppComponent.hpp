#pragma  once

#include "oatpp/web/server/HttpConnectionHandler.hpp"

#include "oatpp/network/tcp/server/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include "oatpp/core/macro/component.hpp"

#include "oatpp-swagger/Controller.hpp"


#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"

class CommonLogInterceptor : public oatpp::web::server::interceptor::ResponseInterceptor {
public:

    std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest> &request,
                                                const std::shared_ptr<OutgoingResponse> &response) override {

        std::string ip("no X-Forwarded-For");
        std::string ua("no user-agent");
        for (const auto &pair: request->getHeaders().getAll()) {
            if (std::string(static_cast<const char *>(pair.first.getData())) == "X-Forwarded-For") {
                ip = static_cast<const char *>(pair.second.getData());
            }
            if (std::string(static_cast<const char *>(pair.first.getData())) == "User-Agent") {
                ua = static_cast<const char *>(pair.second.getData());
            }
        }

        if (ip == "no X-Forwarded-For") {
            auto &context = request->getConnection()->getInputStreamContext();
            for (const auto &pair: context.getProperties().getAll()) {
                if (std::string(static_cast<const char *>(pair.first.getData())) == "peer_address") {
                    ip = static_cast<const char *>(pair.second.getData());
                }
            }
        }

        std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::stringstream ss;
        ss << std::put_time(std::localtime(&t), "%d/%b/%Y:%H:%M:%S %z");//10/Oct/2000:13:55:36 -0700

        auto httpRequest =
                request->getStartingLine().method.std_str() + " " + request->getStartingLine().path.std_str() + " " +
                request->getStartingLine().protocol.std_str();
        OATPP_LOGI("CommonLogInterceptor", "%s - - [%s] \"%s\" %d %ld \"%s\"", ip.c_str(), ss.str().c_str(),
                   httpRequest.c_str(), response->getStatus().code,
                   response->getBody()->getKnownSize(), ua.c_str());

        return response;
    }

};

/**
 *  Class which creates and holds Application components and registers components in oatpp::base::Environment
 *  Order of components initialization is from top to bottom
 */
class AppComponent {
public:

    /**
     *  Create ConnectionProvider component which listens on the port
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, serverConnectionProvider)([] {
        return oatpp::network::tcp::server::ConnectionProvider::createShared(
                {"0.0.0.0", 8000, oatpp::network::Address::IP_4}, true);
    }());

    /**
     *  Create Router component
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, httpRouter)([] {
        return oatpp::web::server::HttpRouter::createShared();
    }());

    /**
     *  Create ConnectionHandler component which uses Router component to route requests
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, serverConnectionHandler)([] {
        OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router); // get Router component
        auto connectionHandler = oatpp::web::server::HttpConnectionHandler::createShared(router);
        connectionHandler->addResponseInterceptor(std::make_shared<CommonLogInterceptor>());
        return connectionHandler;
    }());

    /**
     *  Create ObjectMapper component to serialize/deserialize DTOs in Contoller's API
     */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, apiObjectMapper)([] {
        return oatpp::parser::json::mapping::ObjectMapper::createShared();
    }());

    /**
 *  General API docs info
 */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::DocumentInfo>, swaggerDocumentInfo)([] {

        oatpp::swagger::DocumentInfo::Builder builder;

        builder
                .setTitle("Blackjack Gaming Service")
                .setDescription("A REST API to play Blackjack. Written in C++ with oat++.")
                .setVersion("1.0")
                .setContactName("Oli Zimpasser")
                .setContactUrl("https://bj.oglimmer.de")

                .setLicenseName("Apache License, Version 2.0")
                .setLicenseUrl("http://www.apache.org/licenses/LICENSE-2.0")

                .addServer("https://bj.oglimmer.de", "server on oglimmer.de")
                .addServer("http://localhost:8000", "server on localhost");

        return builder.build();

    }());


/**
 *  Swagger-Ui Resources (<oatpp-examples>/lib/oatpp-swagger/res)
 */
    OATPP_CREATE_COMPONENT(std::shared_ptr<oatpp::swagger::Resources>, swaggerResources)([] {
        // Make sure to specify correct full path to oatpp-swagger/res folder !!!
        return oatpp::swagger::Resources::loadResources("/usr/local/include/oatpp-1.3.0/bin/oatpp-swagger/res");
    }());


};
