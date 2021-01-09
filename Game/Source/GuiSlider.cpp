#include "GuiSlider.h"
#include "Audio.h"
#include "App.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->text = text;
    slider.x = bounds.x;
    slider.y = bounds.y;
    slider.w = 10;
    slider.h = bounds.h;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(Input* input, float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        int mouseX, mouseY;
        input->GetMousePosition(mouseX, mouseY);

        // Check collision between mouse and button bounds
        if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) && 
            (mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
        {
            if (state == GuiControlState::NORMAL)
                app->audio->PlayFx(fxHover);

            state = GuiControlState::FOCUSED;

            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                int posX, posY;
                input->GetMousePosition(posX, posY);
                slider.x = posX;
            }

            if (slider.x < bounds.x)
                slider.x = bounds.x;
            if (slider.x + slider.w >= bounds.x + bounds.w)
                slider.x = bounds.x + bounds.w - slider.w;
            
            float percentage = (100.0f / bounds.w) * (slider.x - bounds.x);
            percentage = percentage / 100.0f;
            value = maxValue * percentage;


            // If mouse button pressed -> Generate event!
            if (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
            {
                app->audio->PlayFx(fxClick);
                NotifyObserver();
            }
        }
        else state = GuiControlState::NORMAL;
    }

    return false;
}

bool GuiSlider::DrawDebug(Render* render)
{
    // DrawDebug the right button depending on state
    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(bounds, { 100, 100, 100, 255 });
        break;
    case GuiControlState::NORMAL: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    case GuiControlState::FOCUSED: render->DrawRectangle(bounds, { 255, 255, 0, 255 });
        break;
    case GuiControlState::PRESSED: render->DrawRectangle(bounds, { 0, 255, 255, 255 });
        break;
    case GuiControlState::SELECTED: render->DrawRectangle(bounds, { 0, 255, 0, 255 });
        break;
    default:
        break;
    }
    render->DrawRectangle(slider, { 255, 0, 0, 255 });
    return false;
}

bool GuiSlider::DrawTexture(Render* render)
{
    switch (state)
    {
    case GuiControlState::NORMAL:
    {
        render->DrawTexture(this->texture, this->slider.x, this->slider.y, &this->anim.frames[0], 0.0f);
        break;
    }
    case GuiControlState::FOCUSED:
    {
        render->DrawTexture(this->texture, this->slider.x, this->slider.y, &this->anim.frames[1], 0.0f);
        break;
    }
    case GuiControlState::PRESSED:
    {
        render->DrawTexture(this->texture, this->slider.x, this->slider.y, &this->anim.frames[2], 0.0f);
        break;
    }
    default:
        break;
    }
    return false;
}
