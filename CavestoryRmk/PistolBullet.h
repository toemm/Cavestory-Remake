#pragma once

#include "Ammunition.h"


class PistolBullet :
	public Bullet
{
public:
	PistolBullet(Graphics& graphics);

	// virtual functs
	virtual void update(int elapsedTime) override;
	virtual void draw(Graphics& graphics, int x, int y) override;

	// pure virtual functs
	virtual void handleTileCollisions(std::vector<Rectangle>& others) override;
	virtual void handleEnemyCollisions(std::vector<std::shared_ptr<Enemy>>& others) override;
	virtual void animationDone(std::string currentAnimation) override;
	virtual void setupAnimation() override;

private:


};

