#include "Bowstring/Application.h"

class HelloScreen : public bowstring::Application {
  using bowstring::Application::Application;

public:
  void onInit() override { this->setClearColor({1.0f, 1.0f, 1.0f, 1.0f}); }

  void onUpdate(float) override {}
};

int main() {
  auto appConfig = bowstring::ApplicationConfig{};
  appConfig.height = 800;
  appConfig.width = 600;
  appConfig.title = "HelloScreen";

  HelloScreen app(appConfig);

  app.run();
}
