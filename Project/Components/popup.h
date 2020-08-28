#ifndef POPUP_H
#define POPUP_H

#include <Ossium.h>

using namespace Ossium;

struct TextBoxMessage
{
    std::string text;
    std::string iconLeftPath;
    std::string iconRightPath;
};

struct PopupSchema : public Schema<PopupSchema, 20>
{
    DECLARE_BASE_SCHEMA(PopupSchema, 20);

    // The delay in seconds per character of the text string shown.
    M(float, textBoxDelay) = 0.032;

};

class Popup : public Component, public PopupSchema
{
public:
    CONSTRUCT_SCHEMA(Component, PopupSchema);
    DECLARE_COMPONENT(Component, Popup);

    void OnLoadFinish();

    void QueueMessage(std::string text, std::string iconLeftPath = "", std::string iconRightPath = "");

    void NextMessage();

    void Update();

    bool IsShown();

private:
    void Show();
    void Hide();

    void Setup();

    bool shown = true;

    std::queue<TextBoxMessage> textBoxQueue;

    bool goToNextMessage = false;

    Uint32 charsWritten = 0;

    Delta delta;

    float seconds = 0;

    Text* textBoxText = nullptr;
    Texture* textBoxIconLeft = nullptr;
    Texture* textBoxIconRight = nullptr;
    Texture* textBoxBackground = nullptr;

};

#endif // POPUP_H
