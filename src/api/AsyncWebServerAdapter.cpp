#include "WebServer.h"
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <vector>
#include <map>

namespace {
    // Classe d'implémentation du routeur
    class AsyncRouter : public WebServer::IRouter {
    private:
        AsyncWebServer& server;
        std::vector<WebServer::Route> routes;

    public:
        AsyncRouter(AsyncWebServer& s) : server(s) {}

        void addRoute(const WebServer::Route& route) override {
            server.on(route.path.c_str(), 
                     static_cast<WebRequestMethod>(route.method), 
                     route.handler,
                     route.uploadHandler,
                     route.bodyHandler);
            routes.push_back(route);
        }

        void removeRoute(const String& path) override {
            auto it = std::find_if(routes.begin(), routes.end(),
                [&path](const WebServer::Route& r) { return r.path == path; });
            if (it != routes.end()) {
                // Note: AsyncWebServer ne supporte pas nativement la suppression des routes
                // Nous gardons la trace pour une éventuelle réimplémentation future
                routes.erase(it);
            }
        }

        void setErrorHandler(WebServer::RequestCallback handler) override {
            server.onNotFound([handler](AsyncWebServerRequest* request) {
                handler(request);
            });
        }

        void setNotFoundHandler(WebServer::RequestCallback handler) override {
            server.onNotFound([handler](AsyncWebServerRequest* request) {
                handler(request);
            });
        }
    };

    // Classe d'implémentation du gestionnaire WebSocket
    class AsyncWebSocketHandler : public WebServer::IWebSocketHandler {
    private:
        AsyncWebSocket ws;

    public:
        AsyncWebSocketHandler(const String& path) : ws(path) {}

        void onConnect(std::function<void(AsyncWebSocket*, AsyncWebSocketClient*)> cb) override {
            ws.onEvent([cb](AsyncWebSocket* server, AsyncWebSocketClient* client,
                          AwsEventType type, void* arg, uint8_t* data, size_t len) {
                if (type == WS_EVT_CONNECT) {
                    cb(server, client);
                }
            });
        }

        void onDisconnect(std::function<void(AsyncWebSocket*, AsyncWebSocketClient*)> cb) override {
            ws.onEvent([cb](AsyncWebSocket* server, AsyncWebSocketClient* client,
                          AwsEventType type, void* arg, uint8_t* data, size_t len) {
                if (type == WS_EVT_DISCONNECT) {
                    cb(server, client);
                }
            });
        }

        void onMessage(std::function<void(AsyncWebSocket*, AsyncWebSocketClient*, String)> cb) override {
            ws.onEvent([cb](AsyncWebSocket* server, AsyncWebSocketClient* client,
                          AwsEventType type, void* arg, uint8_t* data, size_t len) {
                if (type == WS_EVT_DATA) {
                    cb(server, client, String((char*)data));
                }
            });
        }

        void broadcast(const String& message) override {
            ws.textAll(message);
        }

        AsyncWebSocket* native() { return &ws; }
    };

    // Classe principale du serveur
    class AsyncWebServerImpl : public WebServer::IServer {
    private:
        std::unique_ptr<AsyncWebServer> server;
        std::unique_ptr<AsyncRouter> routerImpl;
        std::map<String, std::unique_ptr<AsyncWebSocketHandler>> wsHandlers;
        WebServer::Config serverConfig;
        bool running;

    public:
        AsyncWebServerImpl(const WebServer::Config& config)
            : serverConfig(config)
            , running(false) {
            server = std::make_unique<AsyncWebServer>(config.port);
            routerImpl = std::make_unique<AsyncRouter>(*server);

            if (config.corsEnabled) {
                enableCORS(true, config.corsOrigin);
            }
        }

        bool start() override {
            if (!running) {
                server->begin();
                running = true;
            }
            return running;
        }

        void stop() override {
            if (running) {
                // Note: AsyncWebServer ne supporte pas l'arrêt proprement dit
                // Nous marquons simplement comme arrêté
                running = false;
            }
        }

        WebServer::IRouter* router() override {
            return routerImpl.get();
        }

        WebServer::IWebSocketHandler* ws(const String& path) override {
            auto it = wsHandlers.find(path);
            if (it == wsHandlers.end()) {
                auto handler = std::make_unique<AsyncWebSocketHandler>(path);
                server->addHandler(handler->native());
                wsHandlers[path] = std::move(handler);
                return wsHandlers[path].get();
            }
            return it->second.get();
        }

        void enableCORS(bool enable, const String& origin) override {
            if (enable) {
                DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", origin);
                DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
                DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
            } else {
                DefaultHeaders::Instance().removeHeader("Access-Control-Allow-Origin");
                DefaultHeaders::Instance().removeHeader("Access-Control-Allow-Methods");
                DefaultHeaders::Instance().removeHeader("Access-Control-Allow-Headers");
            }
        }

        bool isRunning() const override {
            return running;
        }

        const WebServer::Config& config() const override {
            return serverConfig;
        }
    };
}

namespace WebServer {
    std::unique_ptr<IServer> createServer(const Config& config) {
        return std::make_unique<AsyncWebServerImpl>(config);
    }
}