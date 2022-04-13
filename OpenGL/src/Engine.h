#pragma once

class Engine
{
protected:
    int m_Width, m_Height;

    virtual void Init() = 0;

    virtual void Update(float dt) = 0;

    virtual void Render() = 0;
    virtual void ImGuiRender(float dt) = 0;

    virtual void Close() = 0;

    void SetSize(int width, int height);

public:
    Engine();

    int Run();
};
