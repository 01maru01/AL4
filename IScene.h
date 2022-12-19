#pragma once

class IScene
{
protected:
	virtual void MatUpdate() = 0;

public:
	virtual ~IScene() = default;
	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void LoadResources() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

