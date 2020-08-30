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

#ifndef OSSIUM_EDITOR
    Hide();
#endif // OSSIUM_EDITOR
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
        textBoxBackground->SetAlphaMod(255);
        Setup();
        shown = true;
    }
}

void Popup::Hide()
{
    if (shown)
    {
        textBoxBackground->SetAlphaMod(0);
        Setup();
        shown = false;
    }
}

bool Popup::IsShown()
{
    return shown;
}

void Popup::Setup()
{
    textBoxText->text = "";
    textBoxText->dirty = true;
    if (!textBoxQueue.empty())
    {
        if (!textBoxQueue.front().iconLeftPath.empty())
        {
            textBoxIconLeft->SetSource(GetService<ResourceController>()->Get<Image>(textBoxQueue.front().iconLeftPath, *GetService<Renderer>()));
        }
        if (!textBoxQueue.front().iconRightPath.empty())
        {
            textBoxIconRight->SetSource(GetService<ResourceController>()->Get<Image>(textBoxQueue.front().iconRightPath, *GetService<Renderer>()));
        }
    }
    else
    {
        textBoxIconLeft->SetSource(nullptr);
        textBoxIconRight->SetSource(nullptr);
    }
    if (textBoxIconLeft->GetSource() == nullptr)
    {
        textBoxIconLeft->width = 0;
        textBoxIconLeft->height = 0;
    }
    if (textBoxIconRight->GetSource() == nullptr)
    {
        textBoxIconRight->width = 0;
        textBoxIconRight->height = 0;
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
        else if (goToNextMessage && !textBoxQueue.front().text.empty())
        {
            charsWritten = textBoxQueue.front().text.size() - 1;
        }

        if (!textBoxQueue.empty())
        {
            if (charsWritten < textBoxQueue.front().text.size() && seconds > textBoxDelay)
            {
                charsWritten++;
                textBoxText->text = textBoxQueue.front().text.substr(0, charsWritten);
                textBoxText->dirty = true;
                seconds = 0;
            }
            seconds += delta.Time();
        }
        else
        {
            Hide();
        }
    }
    goToNextMessage = false;
    delta.Update();
}

