#include <Ossium.h>
#include <OssiumEditor.h>

using namespace Ossium;
using namespace Ossium::Editor;
using namespace std;

int main(int argc, char* argv[])
{
    InitialiseOssium();

#ifdef OSSIUM_EDITOR
    ResourceController resources;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    EditorController* editor = new EditorController(&resources);

    while (editor->Update())
    {
        // Update until the editor stops running.
    }

    resources.FreeAll();

    delete editor;
    editor = nullptr;
#else // Open new scope such that we clean up objects in the correct order.
{
    Config config;
    config.windowTitle = "One More Ghost!";
    config.startScene = argc > 1 ? argv[1] : "assets/scenes/MainMenu.rawr";
    config.windowWidth = 1024;
    config.windowHeight = 768;

    EngineSystem engine(config);

    while (engine.Update())
    {
        // Do nothing
    }

}
#endif // OSSIUM_EDITOR

    TerminateOssium();
    return 0;
}
