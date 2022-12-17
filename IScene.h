#pragma once

class IScene
{
protected:
	virtual void MatUpdate() = 0;

public:
	virtual ~IScene() {};
	virtual void Initialize() = 0;
	virtual void LoadResources() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

