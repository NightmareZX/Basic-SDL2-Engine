#pragma once

enum ValidLayers
{
	//Vital layers
	COLLISION,
	FOREGROUND,
	//Semi Vital
	BACKGROUND,
	//Optional
	PARALAX1,
	PARALAX2,

	NONEVALID
};

class Layer
{
private:
protected:
public:
	Layer(){}
	virtual void Render() = 0;
	virtual void Update() = 0;
};

