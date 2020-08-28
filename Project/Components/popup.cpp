#include "popup.h"

REGISTER_COMPONENT(Popup);

using namespace Ossium;
using namespace std;

void Popup::OnLoadFinish()
{
    delta.Init();
    textBoxBackground = entity->GetComponent<Texture>();
    Entity* found = entity->Find("Text", entity);
    textBoxText = found != nullptr ? found->GetComponent<Text>() : nullptr;
    found = entity->Find("IconLeft", entity);
    textBoxIconLeft = found != nullptr ? found->GetComponent<Texture>() : nullptr;
    found = entity->Find("IconRight", entity);
    textBoxIconRight = found != nullptr ? found->GetComponent<Texture>() : nullptr;
}

void Popup::QueueMessage(string text, string iconLeftPath, string iconRightPath)
{
    TextBoxMessage message = { .text=text, .iconLeftPath=iconLeftPath, .iconRightPath=iconRightPath };
    textBoxQueue.push(message);
}

void Popup::Show()
{
    if (!shown && !textBoxQueue.empty())
    {
        textBoxIconLeft->SetAlphaMod(255);
        textBoxIconRight->SetAlphaMod(255);
        textBoxBackground->SetAlphaMod(255);
        Setup();
        shown = true;
    }
}

void Popup::Hide()
{
    if (shown)
    {
        textBoxIconLeft->SetAlphaMod(0);
        textBoxIconRight->SetAlphaMod(0);
        textBoxBackground->SetAlphaMod(0);
        textBoxText->text = "";
        textBoxText->dirty = true;
        shown = false;
    }
}

void Popup::Setup()
{
    textBoxText->text = "";
    textBoxText->dirty = true;
    textBoxIconLeft->SetSource(GetService<ResourceController>()->Get<Image>(textBoxQueue.front().iconLeftPath, *GetService<Renderer>()));
    textBoxIconRight->SetSource(GetService<ResourceController>()->Get<Image>(textBoxQueue.front().iconRightPath, *GetService<Renderer>()));
    if (textBoxIconLeft->GetSource() == nullptr)
    {
        textBoxIconLeft->SetWidth(0);
        textBoxIconLeft->SetHeight(0);
    }
    if (textBoxIconRight->GetSource() == nullptr)
    {
        textBoxIconRight->SetWidth(0);
        textBoxIconRight->SetHeight(0);
    }
    seconds = 0;
}

void Popup::NextMessage()
{
    goToNextMessage = true;
}

void Popup::Update()
{
    if (goToNextMessage && !shown)
    {
        Show();
        goToNextMessage = false;
    }
    if (!textBoxQueue.empty() && textBoxText != nullptr && shown)
    {
        if (charsWritten >= textBoxQueue.front().text.size())
        {
            if (goToNextMessage)
            {
                textBoxQueue.pop();
                charsWritten = 0;
                Setup();
            }
        }
        else if (goToNextMessage)
        {
            charsWritten = textBoxQueue.front().text.size();
        }

        if (textBoxQueue.empty())
        {
            // Hide the text box
            Hide();
        }
        else if (charsWritten < textBoxQueue.front().text.size() && seconds > textBoxDelay)
        {
            charsWritten++;
            textBoxText->text = textBoxQueue.front().text.substr(0, charsWritten);
            textBoxText->dirty = true;
            seconds = 0;
        }
        goToNextMessage = false;
        seconds += delta.Time();
    }
}

